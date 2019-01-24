//
// Created by slist on 12/13/18.
//

#include <Battle/Battle.h>
#include <Battle/Field.h>
#include <Battle/Party.h>
#include <fileIO/loadJSON.h>
#include <Pokemon/Pokemon.h>
#include <Pokemon/Type.h>
#include <Utils/Logging.h>

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>

Battle::Battle() = default;

Battle::Battle(long seed)
{
    Battle::generator = std::mt19937(seed);
}

Party* Battle::get_party(Players player)
{
    return &Parties[player];
}

Attack_Result Battle::send_out(FIELD_POSITION pos, int poke_position)
{
    if(poke_position < -1 || poke_position > 5)
        ERR_MSG("Invlaid party position\n");

    if(poke_position == -1)
        return Attack_Result::NO_ATTACK;

    Players player = get_player_from_position(pos);
    DEBUG_MSG("Sending out P" << player + 1 << "'s " << Parties[player].party_pokes[poke_position].get_species() << "\n");

    Parties[player].party_pokes[poke_position].to_be_swapped = false;

    // reset status turns for badly poisoned
    if(Parties[player].party_pokes[poke_position].get_status() == STATUS::BADLY_POISONED)
        Parties[player].party_pokes[poke_position].status_turns = 0;

    // if send out failed, the poke fainted and the poke's hp was 0, handle a faint and return false
    if(!Battle::active_field.send_out(pos, &Parties[player].party_pokes[poke_position])
     && Battle::active_field.active_pokes[pos]->get_current_hp() == 0)
    {
        Battle::handle_faint(pos);
        return Attack_Result::FAINT;
    }
    return Attack_Result::HIT;
}

void Battle::return_poke(FIELD_POSITION pos)
{

    DEBUG_MSG("Returning " << Battle::active_field.active_pokes[pos]->get_species() << "\n");
    Battle::active_field.active_pokes[pos]->clear_stat_mods();
    Battle::active_field.active_pokes[pos]->clear_volatile_statuses();
    Battle::active_field.active_pokes[pos]->reset_types();
    Battle::active_field.active_pokes[pos]->reset_protect();
    Battle::active_field.return_poke(pos);
};

Attack_Result Battle::swap_poke(FIELD_POSITION pos, int poke_position)
{

    Battle::return_poke(pos);
    return Battle::send_out(pos, poke_position);
}

Attack_Result Battle::attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move_number)
{
    Attack_Result res;
    bool crit = false;
    Move* move;

    res = Battle::handle_pre_attack_status(atk_pos);
    if(res != Attack_Result::HIT)
        return res;

    res = Battle::handle_v_status_mask(atk_pos, pre_attack_v_status_mask, move_number);
    if(res != Attack_Result::HIT)
        return res;

    if(move_number < 4)
    {
        // Check if there is enough pp to use the move
        if(!Battle::active_field.active_pokes[atk_pos]->use_move(move_number))
        {
            ERR_MSG("Not enough PP\n");
        }
        move = &Battle::active_field.active_pokes[atk_pos]->moves[move_number];
    }
    else
        move = &Battle::game_moves[Game_Moves::MOVE_STRUGGLE];


    DEBUG_MSG("P" << get_player_from_position(atk_pos) + 1  << "'s " << Battle::active_field.active_pokes[atk_pos]->get_species()
           << " used " << move->get_name() << "\n");

    if(Battle::roll_chance(move->get_crit()))
        crit = true;

    if(def_pos == FIELD_POSITION::ALL_TARGETS)
    {
        Battle::Battle_Targets.get_valid_targets(move->get_move_targets(), atk_pos);
        for(int i = 0; i < Battle::Battle_Targets.get_num_valid_targets(); i++)
        {
            if(Battle::attack_target(
                    atk_pos,
                    Battle::Battle_Targets.valid_targets[i],
                    move,
                    crit)
                    == Attack_Result::FAINT)
                res = Attack_Result::FAINT;
        }
    }
    else
        res = Battle::attack_target(atk_pos, def_pos, move, crit);

    return res;
}

Attack_Result Battle::attack_target(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move, bool crit)
{
    std::pair<Attack_Result, float> res;

    if(Battle::active_field.active_pokes[def_pos] == nullptr)
    {
        DEBUG_MSG("Attack Failed due to no pokemon being in slot: " << get_string_from_field_position(def_pos) << std::endl);
        return Attack_Result::NO_ATTACK;
    }

    // roll for hit
    if(!Battle::roll_acc(move->get_acc(),
                         Battle::active_field.active_pokes[atk_pos]->get_stat(STAT::ACC),
                         Battle::active_field.active_pokes[def_pos]->get_stat(STAT::EVA)))
    {
        DEBUG_MSG(move->get_name() << " missed\n");
        return Attack_Result::MISS;
    }

    if(Battle::active_field.active_pokes[def_pos]->is_protected() && !move->ignores_protect())
    {
        DEBUG_MSG(Battle::active_field.active_pokes[def_pos]->get_species() << " is protected\n");
        return Attack_Result::NO_ATTACK;
    }

    switch(move->get_damage_type())
    {
        case move_damage_type::MOVE_PHYSICAL:
        case move_damage_type::MOVE_SPECIAL:
            res = Battle::attack_damage(atk_pos, def_pos, move, crit);
            if(res.first == Attack_Result::IMMUNE)
                return res.first;
            break;;
        case move_damage_type::MOVE_STATUS:
            res.first = Attack_Result::NO_ATTACK;
            break;
        default:
            ERR_MSG("Unhandled attack type" << move->get_damage_type() << "\n");
    }

    // Handling move effects
    for(int i = 0; i < MAX_EFFECTS; i++)
    {
        if (move->get_move_effect(i).get_effect() != NO_MOVE_EFFECT)
        {
            res.first = Battle::handle_move_effects(
                    move->get_move_effect(i), atk_pos, def_pos, res.second);
            if (res.first == Attack_Result::SWAP)
                return res.first;
        }
        else
            break;
    }

    if(move->makes_contact())
    {
        if(res.first == Attack_Result::HIT)
        {
            res.first = Battle::handle_contact(atk_pos, def_pos);
        }
        else
            Battle::handle_contact(atk_pos, def_pos);
    }

    // Handle returning faint if needed
    if(!Battle::active_field.position_alive(def_pos) || !Battle::active_field.position_alive(atk_pos))
        return Attack_Result::FAINT;

    return res.first;
}


std::pair<Attack_Result, float> Battle::attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move, bool crit)
{
    float eff_atk, eff_def, damage_dealt, move_power;
    float damage_mod;

    damage_mod = Battle::calculate_damage_modifier(move, Battle::active_field.active_pokes[atk_pos], Battle::active_field.active_pokes[def_pos], move->get_num_targets(), crit);

    if(damage_mod == 0)
    {
        DEBUG_MSG("P" << get_player_from_position(def_pos) + 1  << "'s " << Battle::active_field.active_pokes[def_pos]->get_species()
                  << " is immune to " << type_to_string(move->get_type()) << " type moves\n");
        return std::make_pair(Attack_Result::IMMUNE, 0);
    }

    if(move->get_power() == 0)
        return std::make_pair(Attack_Result::HIT, 0);

    // determine the effective stats to use
    if(move->get_damage_type() == move_damage_type::MOVE_PHYSICAL)
    {
        eff_atk = Battle::active_field.active_pokes[atk_pos]->get_stat(STAT::ATK);
        eff_def = Battle::active_field.active_pokes[def_pos]->get_stat(STAT::DEF);
    }
    else
    {
        eff_atk = Battle::active_field.active_pokes[atk_pos]->get_stat(STAT::SPA);
        eff_def = Battle::active_field.active_pokes[def_pos]->get_stat(STAT::SPD);
    }

    move_power = Battle::get_move_power(atk_pos, def_pos, move);

    // calculate damage dealt
    damage_dealt = Battle::calculate_damage_dealt(Battle::active_field.active_pokes[atk_pos]->get_level(), move_power, eff_atk, eff_def, damage_mod);

    // Deal damage and handle fainting but DO NOT RETURN FAINT RESULT UNTIL AFTER SWAP HAS A CHANCE TO RETURN
    if(!Battle::active_field.active_pokes[def_pos]->deal_damage(damage_dealt))
    {
        Battle::handle_faint(def_pos);
        return std::make_pair(Attack_Result::FAINT, damage_dealt);
    }

    return std::make_pair(Attack_Result::HIT, damage_dealt);
}

int Battle::get_move_power(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move)
{
    if(move->get_power() != -1)
        return move->get_power();

    if(move->get_name() == "Gyro_Ball")
    {
        return (25.0 * Battle::active_field.active_pokes[def_pos]->get_stat(STAT::SPE) /
                Battle::active_field.active_pokes[atk_pos]->get_stat(STAT::SPE)) + 1;
    }

    ERR_MSG("Unhandled move power equation\n");
}

Attack_Result Battle::handle_contact(FIELD_POSITION attacker, FIELD_POSITION defender)
{
    return Attack_Result::HIT;
}

Attack_Result Battle::handle_move_effects(Effect move_effect, FIELD_POSITION atk_pos, FIELD_POSITION def_pos, float damage)
{
    FIELD_POSITION effect_target;
    if(move_effect.does_target_self())
        effect_target = atk_pos;
    else
        effect_target = def_pos;

    switch(move_effect.get_effect())
    {
        case MOVE_EFFECTS::SWAP:
            return Attack_Result::SWAP;
        case MOVE_EFFECTS::FLINCH:
            if(Battle::roll_chance(move_effect.get_effect_chance()))
            {
                return Attack_Result::FLINCHED;
            }

            return Attack_Result::MISS;
        case MOVE_EFFECTS::NON_VOLATILE_STATUS_EFFECT:
            if(Battle::roll_chance(move_effect.get_effect_chance()))
            {
                if(Battle::active_field.active_pokes[effect_target]->set_status(move_effect.get_effect_status_type()))
                    DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                              << Battle::active_field.active_pokes[effect_target]->get_species()
                              << " is now " << status_to_string(move_effect.get_effect_status_type()) << "\n");
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::VOLATILE_STATUS_EFFECT:
            if(Battle::roll_chance(move_effect.get_effect_chance()))
            {
                if(Battle::active_field.active_pokes[effect_target]->set_volatile_status(move_effect.get_volatile_status_effect()))
                {
                    DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                              << Battle::active_field.active_pokes[effect_target]->get_species()
                              << " is now " << v_status_to_string(move_effect.get_volatile_status_effect()) << "\n");

                }
                return Attack_Result::HIT;
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::STAT_CHANGE:
            Battle::active_field.active_pokes[effect_target]->stat_change(move_effect.get_stat_changed(), move_effect.get_stages_changed());

            return Attack_Result::HIT;
        case MOVE_EFFECTS::FIELD_CHANGE:
            Battle::active_field.modify_field_obj(move_effect.get_field_obj_changed(), def_pos, atk_pos);

            return Attack_Result::HIT;
        case MOVE_EFFECTS::RECOIL:
            DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                      << Battle::active_field.active_pokes[effect_target]->get_species() << " was hurt by recoil\n");
            if(move_effect.get_use_damage())
                Battle::active_field.active_pokes[effect_target]->deal_damage(damage * move_effect.get_percent_recoil());
            else
                Battle::active_field.active_pokes[effect_target]->deal_damage(Battle::active_field.active_pokes[effect_target]->get_stat(STAT::HP)
                                                                     * move_effect.get_percent_recoil());
            if(Battle::active_field.position_alive(effect_target))
            {
                Battle::handle_faint(effect_target);
                return Attack_Result::FAINT;
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::HEAL:
            Battle::active_field.active_pokes[effect_target]->heal_damage(Battle::active_field.active_pokes[effect_target]->get_stat(STAT::HP) * move_effect.get_heal_percent());

            return Attack_Result::HIT;
        case MOVE_EFFECTS::REMOVE_TYPE:
            if(Battle::active_field.active_pokes[effect_target]->get_type()[0] == move_effect.get_type_removed())
                Battle::active_field.active_pokes[effect_target]->remove_type(0);
            else if(Battle::active_field.active_pokes[effect_target]->get_type()[1] == move_effect.get_type_removed())
                Battle::active_field.active_pokes[effect_target]->remove_type(1);

            return Attack_Result::HIT;
        case MOVE_EFFECTS::FLAT_DAMAGE:
            int damage;
            if(move_effect.use_flat_level())
            {
                damage = Battle::active_field.active_pokes[atk_pos]->get_level();
            }
            else
            {
                damage = move_effect.get_flat_damage();
            }

            if(!Battle::active_field.active_pokes[effect_target]->deal_damage(damage))
            {
                Battle::handle_faint(effect_target);
                return Attack_Result::FAINT;
            }
            else
            {
                return Attack_Result::HIT;
            }
        case MOVE_EFFECTS::PROTECT:
            Battle::active_field.active_pokes[effect_target]->increment_protect_turns();
            if(Battle::roll_chance(1.0/Battle::active_field.active_pokes[effect_target]->get_protect_turns()))
            {
                Battle::active_field.active_pokes[effect_target]->protect_poke();
                return Attack_Result::HIT;
            }
            else
            {
                DEBUG_MSG("But it failed\n");
                return Attack_Result::MISS;
            }
        default:
            ERR_MSG("Unhandled move effect " << move_effect.get_effect() << "\n");
    }
    ERR_MSG("Handling of move effect " << move_effect.get_effect() << " did not return correctly\n");
}

bool Battle::has_lost(Players player)
{
    for(int i = 0; i < 6; i++)
    {
        if(Battle::Parties[player].party_pokes[i].is_alive())
        {
            return false;
        }
    }

    return true;
}

bool Battle::can_swap(Players player)
{
    for(int i = 0; i < 6; i++)
    {
        if(Battle::Parties[player].party_pokes[i].is_alive() && !Battle::Parties[player].party_pokes[i].is_active())
            return true;
    }
    return false;
}

void Battle::reset_temp_field_status()
{
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        if(Battle::active_field.active_pokes[i] == nullptr)
            continue;

        Battle::active_field.active_pokes[i]->reset_types();

        //TODO: DOESNT WORK IN DOUBLES CORRECTLY DUE TO WIDEGAURD AND THE LIKE
        if(!Battle::active_field.active_pokes[i]->is_protected())
            Battle::active_field.active_pokes[i]->clear_protect_turns();
        Battle::active_field.active_pokes[i]->reset_protect();

#if BATTLE_TYPE != SINGLE_BATTLE
        //WARN_MSG("Protect clearing is only properly supported in single battles due to team wide protects\n");
#endif
    }
}

float Battle::calculate_damage_dealt(int attacker_level, int move_power, float atk, float def, float damage_modifier)
{
    float base_damage = ((((2 * (float)attacker_level / 5) + 2) * (float)move_power * atk / def / 50) + 2) * damage_modifier;
    float damage_adjustment = std::uniform_int_distribution<int>{85, 100}(Battle::generator) / 100.0;

    return base_damage * damage_adjustment;
}

void Battle::handle_faint(FIELD_POSITION pos)
{
    Battle::active_field.active_pokes[pos]->faint_poke();
    DEBUG_MSG("P" << (get_player_from_position(pos) + 1) << "'s "
           << Battle::active_field.active_pokes[pos]->get_species() << " FAINTED" << "\n");
}

Attack_Result Battle::handle_pre_attack_status(FIELD_POSITION pos)
{
    switch(Battle::active_field.active_pokes[pos]->get_status())
    {
        case STATUS::ASLEEP:
            DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is asleep\n");
            if(Battle::active_field.active_pokes[pos]->status_turns == 0)
            {
                Battle::active_field.active_pokes[pos]->status_turns++;
                return Attack_Result::NO_ATTACK;
            }
            else if(Battle::active_field.active_pokes[pos]->status_turns < 3)
            {
                if(Battle::roll_chance((float)2/3))
                {
                    Battle::active_field.active_pokes[pos]->status_turns++;
                    return Attack_Result::NO_ATTACK;
                }
                else
                {
                    DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " woke up!\n");
                    Battle::active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
                }
            }
            else
            {
                DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " woke up!\n");
                Battle::active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
            }
            return Attack_Result::HIT;
        case STATUS::PARALYZED:
            DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is paralyzed and may not be able to attack\n");
            if(Battle::roll_chance(0.25))
            {
                DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is paralyzed\n");
                return Attack_Result::NO_ATTACK;
            }

            return Attack_Result::HIT;
        case STATUS::FROZEN:
            if(Battle::roll_chance(0.8))
            {
                DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is frozen\n");
                return Attack_Result::NO_ATTACK;
            }

            DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " thawed!\n");
            Battle::active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
            return Attack_Result::HIT;
        default:
            return Attack_Result::HIT;
    }
}

bool Battle::handle_end_turn_field_status()
{
    bool fainted = false;
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        if(Battle::active_field.active_pokes[i] == nullptr)
            continue;
        else if(!Battle::handle_end_turn_statuses(static_cast<FIELD_POSITION>(i)))
        {
            Battle::handle_faint(static_cast<FIELD_POSITION>(i));
            if(Battle::has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
                return true;
            fainted = true;
        }
    }

    return fainted;
}

bool Battle::handle_end_turn_statuses(FIELD_POSITION pos)
{
    if(!Battle::active_field.position_alive(pos))
        return true;

    if(Battle::handle_end_turn_status(pos)
    && !(Battle::handle_v_status_mask(pos, turn_end_v_status_mask) == Attack_Result::FAINT)
    && Battle::active_field.handle_end_turn_field_obj(pos))
        return true;
    else
    {
        Battle::handle_faint(pos);
        return false;
    }
}

bool Battle::handle_end_turn_status(FIELD_POSITION pos)
{
    float damage = 0;
    if(Battle::active_field.active_pokes[pos]->get_status() != STATUS::NO_STATUS
    && Battle::active_field.active_pokes[pos]->get_status() != STATUS::PARALYZED)
        DEBUG_MSG("P" << get_player_from_position(pos) + 1 << "'s " << Battle::active_field.active_pokes[pos]->get_species() << " is");

    switch(Battle::active_field.active_pokes[pos]->get_status())
    {
        case STATUS::BURNED:
            DEBUG_MSG(" burned\n");
            DEBUG_MSG("Burn ");
            damage = Battle::active_field.active_pokes[pos]->get_stat(STAT::HP) / 16.0;
            break;;
        case STATUS::POISONED:
            DEBUG_MSG(" poisoned\n");
            DEBUG_MSG( "Poison ");
            damage = Battle::active_field.active_pokes[pos]->get_stat(STAT::HP) / 8.0;
            break;;
        case STATUS::BADLY_POISONED:
            DEBUG_MSG(" badly poisoned\n");
            DEBUG_MSG("Poison ");
            Battle::active_field.active_pokes[pos]->status_turns++;
            damage = Battle::active_field.active_pokes[pos]->get_stat(STAT::HP) / 16.0 * Battle::active_field.active_pokes[pos]->status_turns;
            break;;
        default:
            return true;
    }
    return Battle::active_field.active_pokes[pos]->deal_damage(damage);
}

Attack_Result Battle::handle_v_status_mask(FIELD_POSITION pos, int status_mask, int move_num)
{
    int current_v_status, i = 0;
    Attack_Result res = Attack_Result::HIT;
    for(int i = 0; i < VOLATILE_STATUS_NUMBERS::NUM_VOLATILE_STATUS; i++)
    {
        current_v_status = (1u << i);
        if((current_v_status & status_mask) && (current_v_status & Battle::active_field.active_pokes[pos]->get_volatile_status()))
        {
            res = Battle::handle_v_status(pos, current_v_status, move_num);
            if(res != Attack_Result::HIT)
                break;
        }
    }
    return res;
}

Attack_Result Battle::handle_v_status(FIELD_POSITION pos, int v_status, int move_num)
{
    switch(v_status)
    {
        case VOLATILE_STATUS::CONFUSION:
            DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is confused\n");
            switch(Battle::active_field.active_pokes[pos]->get_v_status_turns(NUM_CONFUSION))
            {
                case 0:
                    Battle::active_field.active_pokes[pos]->increment_v_status_turns(NUM_CONFUSION);
                    break;;
                case 1:
                case 2:
                case 3:
                    if(Battle::roll_chance(0.25))
                    {
                        Battle::active_field.active_pokes[pos]->clear_v_status_turns(NUM_CONFUSION);
                        Battle::active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::CONFUSION);
                        DEBUG_MSG( Battle::active_field.active_pokes[pos]->get_species() << " snapped out of its confusion\n");
                        return Attack_Result::HIT;
                    }
                    Battle::active_field.active_pokes[pos]->increment_v_status_turns(NUM_CONFUSION);
                    break;;
                case 4:
                    Battle::active_field.active_pokes[pos]->clear_v_status_turns(NUM_CONFUSION);
                    Battle::active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::CONFUSION);
                    DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " snapped out of its confusion\n");
                    return Attack_Result::HIT;
                default:
                    ERR_MSG("Confusion turn count impossible\n");
            }

            if(roll_chance(Battle::game_moves[Game_Moves::MOVE_CONFUSION].get_acc()))
            {
                DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " hurt itself in its confusion\n");
                if(Battle::attack_damage(pos, pos, &Battle::game_moves[Game_Moves::MOVE_CONFUSION], false).first == Attack_Result::FAINT)
                    return Attack_Result::FAINT;
                else
                    return Attack_Result::NO_ATTACK;
            }

            return Attack_Result::HIT;
        case VOLATILE_STATUS::TAUNTED:
            if(Battle::active_field.active_pokes[pos]->get_v_status_turns(NUM_TAUNTED) < 4)
            {
                Battle::active_field.active_pokes[pos]->increment_v_status_turns(NUM_TAUNTED);
                if(Battle::active_field.active_pokes[pos]->moves[move_num].get_damage_type() == move_damage_type::MOVE_STATUS)
                {
                    DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is taunted and can't use "
                           << Battle::active_field.active_pokes[pos]->moves[move_num].get_name() << std::endl);
                    return Attack_Result::NO_ATTACK;
                }
                return Attack_Result::HIT;
            }
            else if(Battle::active_field.active_pokes[pos]->get_v_status_turns(NUM_TAUNTED) == 4)
            {
                DEBUG_MSG(Battle::active_field.active_pokes[pos]->get_species() << " is no longer taunted\n");
                Battle::active_field.active_pokes[pos]->clear_v_status_turns(NUM_TAUNTED);
                Battle::active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::TAUNTED);
                return Attack_Result::HIT;
            }
            ERR_MSG("Impossible Taunt status handling\n");
        case VOLATILE_STATUS::BOUND:
        case VOLATILE_STATUS::CANT_ESCAPE:
        case VOLATILE_STATUS::CURSE:
        case VOLATILE_STATUS::EMBARGO:
        case VOLATILE_STATUS::ENCORE:
        case VOLATILE_STATUS::FLINCHED:
        case VOLATILE_STATUS::HEALBLOCK:
        case VOLATILE_STATUS::IDENTIFIED:
        case VOLATILE_STATUS::INFATUATION:
        case VOLATILE_STATUS::NIGHTMARE:
        case VOLATILE_STATUS::PERISHSONG:
        case VOLATILE_STATUS::TELEKINESIS:
        case VOLATILE_STATUS::TORMENT:
        default:
            ERR_MSG("Unhandled Volatile status");
    }
    return Attack_Result::HIT;
}

bool Battle::roll_acc(float acc, float atk_acc_mod, float def_eva_mod)
{
    // if acc < 0 its an always hit
    if(acc < 0)
        return true;
    else
    {
        return Battle::roll_chance(acc*atk_acc_mod*(1/def_eva_mod));
    }
}

bool Battle::roll_chance(float chance)
{
    float c = std::uniform_real_distribution<float>{0, 1}(Battle::generator);
    return c < chance;
}

float Battle::calculate_damage_modifier(Move* move, Pokemon* attacker, Pokemon* defender, int num_targets, bool crit)
{
    float damage_modifier = 1;

    if(crit)
    {
        DEBUG_MSG("Critical Hit\n");
        damage_modifier *= 1.5;
    }

    damage_modifier *= calculate_type_damage_modifier(defender->get_type(), move->get_type());

    if(damage_modifier >= 2)
        DEBUG_MSG("It's super effective\n");
    else if(damage_modifier > 0 && damage_modifier < 1)
        DEBUG_MSG("It's not very effective\n");

    if(num_targets > 1)
        damage_modifier *= 0.75;

    if((Battle::active_field.weather_state == Weather::RAIN && move->get_type() == PokeTypes::WATER)
    || (Battle::active_field.weather_state == Weather::HARSH_SUNLIGHT && move->get_type() == PokeTypes::FIRE))
        damage_modifier *= 1.5;

    if((Battle::active_field.weather_state == Weather::RAIN && move->get_type() == PokeTypes::FIRE)
    || (Battle::active_field.weather_state == Weather::HARSH_SUNLIGHT && move->get_type() == PokeTypes::WATER))
        damage_modifier *= 0.5;

    if(is_stab(attacker->get_type(), move->get_type()))
        damage_modifier *= 1.5;

    if(move->get_damage_type() == move_damage_type::MOVE_PHYSICAL && attacker->get_status() == STATUS::BURNED)
        damage_modifier *= 0.5;

    if(move->get_type() == PokeTypes::GROUND && !defender->is_grounded())
        damage_modifier = 0;

    return damage_modifier;
}

// LOADING AND RESETING
void Battle::reset()
{
    Battle::active_field.reset();
    Battle::Parties[Players::PLAYER_ONE].reset();
    Battle::Parties[Players::PLAYER_TWO].reset();
}

void Battle::update_generator(long seed)
{
    Battle::generator = std::mt19937(seed);
}

void Battle::load_battle(std::vector<std::string> team_choice)
{
    Battle::load_teams(team_choice);
    Battle::load_game_moves();
}

void Battle::load_teams(std::vector<std::string> team_names)
{
    boost::property_tree::ptree team_1 = load_json_file("teams/" + team_names[Players::PLAYER_ONE] + ".json");
    boost::property_tree::ptree team_2 = load_json_file("teams/"  + team_names[Players::PLAYER_TWO] + ".json");
    int i = 0;
    bool done [] = {false, false};

    while(!(done[Players::PLAYER_ONE] && done[Players::PLAYER_TWO]) && i < 6)
    {
        if(!done[Players::PLAYER_ONE])
        {
            try
            {
                Parties[Players::PLAYER_ONE].party_pokes[i].load_pokemon(team_1.get_child(std::to_string(i)));
            }
            catch(...)
            {
                DEBUG_MSG("Finished importing " << team_names[Players::PLAYER_ONE] << "\n");
                done[Players::PLAYER_ONE] = true;
            }
        }
        if(!done[Players::PLAYER_TWO])
        {
            try
            {
                Parties[Players::PLAYER_TWO].party_pokes[i].load_pokemon(team_2.get_child(std::to_string(i)));
            }
            catch(...)
            {
                DEBUG_MSG("Finished importing " << team_names[Players::PLAYER_TWO] << "\n");
                done[Players::PLAYER_TWO] = true;
            }
        }
        i++;
    }
}

void Battle::load_game_moves()
{
    Battle::game_moves[Game_Moves::MOVE_CONFUSION].load_move("game_moves/Confusion");
    Battle::game_moves[Game_Moves::MOVE_STRUGGLE].load_move("game_moves/Struggle");
}

void Battle::print_battle(bool detailed)
{
#ifdef DEBUGGING
    Battle::active_field.print_field(detailed);

    DEBUG_MSG("Player ONE party pokemon:\n");
    Battle::Parties[Players::PLAYER_ONE].print_party(detailed);
    DEBUG_MSG("\nPlayer TWO party pokemon:\n");
    Battle::Parties[Players::PLAYER_TWO].print_party(detailed);
    DEBUG_MSG(std::endl);
#endif
}