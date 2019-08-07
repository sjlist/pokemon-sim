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
#include <map>
using namespace std;

static unsigned int pre_attack_v_status_mask = (VOLATILE_STATUS::CONFUSION +
                                                VOLATILE_STATUS::ENCORE +
                                                VOLATILE_STATUS::FLINCHED +
                                                VOLATILE_STATUS::TAUNTED);

static unsigned int turn_end_v_status_mask = 0;

static unsigned int turn_end_v_status_mask_clear = (VOLATILE_STATUS::FLINCHED);

enum VARIABLE_POWER_MOVES {
    GYRO_BALL = 0,
    NUM_VARIABLE_POWER_MOVES
};

map<string, int> variable_move_power_map ={
        {"Gyro_Ball", GYRO_BALL}
};

Battle::Battle()
{
    chance = uniform_real_distribution<float>{0,1};
    damage_calc = uniform_int_distribution<int>{85, 100};
}

Battle::Battle(long seed)
{
    generator = mt19937(seed);
    chance = uniform_real_distribution<float>{0,1};
    damage_calc = uniform_int_distribution<int>{85, 100};
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
    if(!Parties[player].party_pokes[poke_position].is_alive())
        ERR_MSG("Pokemon fained, cannot send out\n");

    if(Parties[player].party_pokes[poke_position].is_active())
        ERR_MSG("Pokemon already active, cannot send out\n");

    DEBUG_MSG("Sending out P" << player + 1 << "'s " << Parties[player].party_pokes[poke_position].get_species() << "\n");

    Parties[player].party_pokes[poke_position].to_be_swapped = false;

    // reset status turns for badly poisoned
    if(Parties[player].party_pokes[poke_position].get_status() == STATUS::BADLY_POISONED)
        Parties[player].party_pokes[poke_position].status_turns = 0;

    // if send out failed, the poke fainted and the poke's hp was 0, handle a faint and return false
    if(!active_field.send_out(pos, &Parties[player].party_pokes[poke_position]))
    {
        handle_faint(pos);
        return Attack_Result::FAINT;
    }
    return Attack_Result::HIT;
}

void Battle::return_poke(FIELD_POSITION pos)
{
    if(active_field.active_pokes[pos] == nullptr)
    {
        ERR_MSG("Can't return a pokemon that doesn't exist");
    }
    DEBUG_MSG("Returning " << active_field.active_pokes[pos]->get_species() << "\n");
    active_field.active_pokes[pos]->clear_stat_mods();
    active_field.active_pokes[pos]->clear_volatile_statuses();
    active_field.active_pokes[pos]->reset_types();
    active_field.active_pokes[pos]->reset_protect();
    active_field.return_poke(pos);
}

Attack_Result Battle::swap_poke(FIELD_POSITION pos, int poke_position)
{
    return_poke(pos);
    return send_out(pos, poke_position);
}

Attack_Result Battle::attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move_number)
{
    Attack_Result res = Attack_Result::HIT, temp_res;
    FIELD_POSITION attack_target_pos;
    Move* move;
    int num_defenders;

    res = handle_pre_attack_status(atk_pos);
    if(res != Attack_Result::HIT)
        return res;

    res = handle_v_status_mask(atk_pos, pre_attack_v_status_mask, move_number);
    if(res != Attack_Result::HIT)
        return res;

    if(move_number < 4)
    {
        // Check if there is enough pp to use the move
        if(!active_field.active_pokes[atk_pos]->use_move(move_number))
        {
            ERR_MSG("Not enough PP\n");
        }
        move = &active_field.active_pokes[atk_pos]->moves[move_number];
    }
    else
        move = &game_moves[Game_Moves::MOVE_STRUGGLE];

    DEBUG_MSG("P" << get_player_from_position(atk_pos) + 1  << "'s " << active_field.active_pokes[atk_pos]->get_species()
           << " used " << move->get_name() << "\n");

    if(move->must_be_used_first_turn() && !active_field.active_pokes[atk_pos]->first_turn)
    {
        DEBUG_MSG("But it failed\n");
        return Attack_Result::NO_ATTACK;
    }

    for(int j = 0; j < move->get_num_hits(); j++)
    {
        Battle_Targets.get_valid_targets(move->get_move_targets(), atk_pos);
        if(def_pos == ALL_TARGETS)
            num_defenders = Battle_Targets.get_num_valid_targets();
        else
            num_defenders = 1;

        for(int i = 0; i < num_defenders; i++)
        {
            if(move->get_num_targets() == 0)
                attack_target_pos = Battle_Targets.valid_targets[i];
            else
                attack_target_pos = def_pos;

            temp_res = attack_target(atk_pos, attack_target_pos, move);
            switch(temp_res)
            {
                case Attack_Result::FAINT:
                    res = temp_res;
                    break;
                case Attack_Result::SWAP:
                    if(res != Attack_Result::FAINT)
                        res = temp_res;
                    break;
                default:
                    break;
            }
        }
    }

    return res;
}

Attack_Result Battle::attack_target(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move)
{
    pair<Attack_Result, float> res;
    Attack_Result temp_res;

    if(active_field.active_pokes[def_pos] == nullptr)
    {
        DEBUG_MSG("Attack Failed due to no pokemon being in slot: " << get_string_from_field_position(def_pos) << endl);
        return Attack_Result::NO_ATTACK;
    }

    if(!active_field.active_pokes[def_pos]->is_alive())
    {
        DEBUG_MSG("Attack Failed due to " << active_field.active_pokes[def_pos]->get_species() << " being fainted" << endl);
        return Attack_Result::FAINT;
    }

    // roll for hit
    if(!roll_acc(move->get_acc(),
                         active_field.active_pokes[atk_pos]->get_stat(STAT::ACC),
                         active_field.active_pokes[def_pos]->get_stat(STAT::EVA)))
    {
        DEBUG_MSG(move->get_name() << " missed\n");
        return Attack_Result::MISS;
    }

    if(active_field.active_pokes[def_pos]->is_protected() && !move->ignores_protect())
    {
        DEBUG_MSG(active_field.active_pokes[def_pos]->get_species() << " is protected\n");
        return Attack_Result::NO_ATTACK;
    }

    switch(move->get_damage_type())
    {
        case move_damage_type::MOVE_PHYSICAL:
        case move_damage_type::MOVE_SPECIAL:
            res = attack_damage(atk_pos, def_pos, move);
            if(res.first == Attack_Result::IMMUNE)
                return res.first;
            break;
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
            temp_res = handle_move_effects(
                    move->get_move_effect(i), atk_pos, def_pos, res.second);

            //if temp result isnt a hit as thats the default so I dont care about it
            // and the current result isn't faint or the temp result is swap, then overwrite the current result
            if(temp_res != Attack_Result::HIT
            && (res.first != Attack_Result::FAINT || temp_res == Attack_Result::SWAP))
                res.first = temp_res;

            // TODO: MAY NOT BE RIGHT WITH CONTACT DAMAGE
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
            res.first = handle_contact(atk_pos, def_pos);
        }
        else
            handle_contact(atk_pos, def_pos);
    }

    // Handle returning faint if needed
    if(!active_field.position_alive(def_pos) || !active_field.position_alive(atk_pos))
        return Attack_Result::FAINT;

    return res.first;
}

int Battle::do_chain_mult(int base_power, vector<float>* mults)
{
    int ret = base_power;
    for(unsigned int i = 0; i < mults->size(); i++)
    {
        ret = floor(ret * mults->at(i));
    }
    return ret;
}

pair<Attack_Result, float> Battle::attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move)
{
    vector<float> damage_mults;
    float eff_atk, eff_def, damage_dealt;
    int move_power;
    bool crit = false;

    if(roll_chance(move->get_crit()))
        crit = true;

    calculate_damage_modifier(&damage_mults, move, active_field.active_pokes[atk_pos], active_field.active_pokes[def_pos], Battle_Targets.get_num_valid_targets(), crit);


    for(unsigned int i = 0; i < damage_mults.size(); i++)
    {
        if(damage_mults.at(i) == 0)
        {
            DEBUG_MSG("P" << get_player_from_position(def_pos) + 1  << "'s " << active_field.active_pokes[def_pos]->get_species()
                          << " is immune to " << type_to_string(move->get_type()) << " type moves\n");
            return make_pair(Attack_Result::IMMUNE, 0);
        }
    }

    if(move->get_power() == 0)
        return make_pair(Attack_Result::HIT, 0);

    // determine the effective stats to use
    if(move->get_damage_type() == move_damage_type::MOVE_PHYSICAL)
    {
        eff_atk = active_field.active_pokes[atk_pos]->get_stat(STAT::ATK);
        eff_def = active_field.active_pokes[def_pos]->get_stat(STAT::DEF);
    }
    else
    {
        eff_atk = active_field.active_pokes[atk_pos]->get_stat(STAT::SPA);
        eff_def = active_field.active_pokes[def_pos]->get_stat(STAT::SPD);
    }

    move_power = get_move_power(atk_pos, def_pos, move);

    // calculate damage dealt
    damage_dealt = calculate_damage_dealt(active_field.active_pokes[atk_pos]->get_level(), move_power, eff_atk, eff_def, &damage_mults);

    // Deal damage and handle fainting but DO NOT RETURN FAINT RESULT UNTIL AFTER SWAP HAS A CHANCE TO RETURN
    if(!active_field.active_pokes[def_pos]->deal_damage(damage_dealt))
    {
        handle_faint(def_pos);
        return make_pair(Attack_Result::FAINT, damage_dealt);
    }

    return make_pair(Attack_Result::HIT, damage_dealt);
}

int Battle::get_move_power(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move)
{
    if(move->get_power() == -1)
    {
        switch (variable_move_power_map[move->get_name()])
        {
            case GYRO_BALL:
                return 25.0 * active_field.active_pokes[def_pos]->get_stat(STAT::SPE) /
                       active_field.active_pokes[atk_pos]->get_stat(STAT::SPE) + 1;
            default:
                ERR_MSG("Unhandled move power equation\n");
        }
    }

    return move->get_power();
}

Attack_Result Battle::handle_contact(FIELD_POSITION attacker, FIELD_POSITION defender)
{
    //NOT IMPLEMENTED YET
    DEBUG_MSG("P" << get_player_from_position(attacker) + 1  << "'s " << active_field.active_pokes[attacker]->get_species()
           << " made contact with "
           << "P" << get_player_from_position(defender) + 1  << "'s " << active_field.active_pokes[defender]->get_species() << endl);
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

        case MOVE_EFFECTS::NON_VOLATILE_STATUS_EFFECT:
            if(roll_chance(move_effect.get_effect_chance()))
            {
                if(active_field.active_pokes[effect_target]->set_status(move_effect.get_effect_status_type()))
                    DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                              << active_field.active_pokes[effect_target]->get_species()
                              << " is now " << status_to_string(move_effect.get_effect_status_type()) << "\n");
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::VOLATILE_STATUS_EFFECT:
            if(roll_chance(move_effect.get_effect_chance()))
            {
                if(active_field.active_pokes[effect_target]->set_volatile_status(move_effect.get_volatile_status_effect()))
                {
                    DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                              << active_field.active_pokes[effect_target]->get_species()
                              << " is now " << v_status_to_string(move_effect.get_volatile_status_effect()) << "\n");

                }
                return Attack_Result::HIT;
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::STAT_CHANGE:
            active_field.active_pokes[effect_target]->stat_change(move_effect.get_stat_changed(), move_effect.get_stages_changed());

            return Attack_Result::HIT;
        case MOVE_EFFECTS::FIELD_CHANGE:
            active_field.modify_field_obj(move_effect.get_field_obj_changed(), def_pos, atk_pos);

            return Attack_Result::HIT;
        case MOVE_EFFECTS::RECOIL:
            DEBUG_MSG("P" << get_player_from_position(effect_target) + 1 << "'s "
                      << active_field.active_pokes[effect_target]->get_species() << " was hurt by recoil\n");
            if(move_effect.get_use_damage())
                active_field.active_pokes[effect_target]->deal_damage(damage * move_effect.get_percent_recoil());
            else
                active_field.active_pokes[effect_target]->deal_damage(active_field.active_pokes[effect_target]->get_stat(STAT::HP)
                                                                     * move_effect.get_percent_recoil());
            if(active_field.position_alive(effect_target))
            {
                handle_faint(effect_target);
                return Attack_Result::FAINT;
            }

            return Attack_Result::HIT;
        case MOVE_EFFECTS::HEAL:
            active_field.active_pokes[effect_target]->heal_damage(active_field.active_pokes[effect_target]->get_stat(STAT::HP) * move_effect.get_heal_percent());

            return Attack_Result::HIT;
        case MOVE_EFFECTS::REMOVE_TYPE:
            if(active_field.active_pokes[effect_target]->get_type()[0] == move_effect.get_type_removed())
                active_field.active_pokes[effect_target]->remove_type(0);
            else if(active_field.active_pokes[effect_target]->get_type()[1] == move_effect.get_type_removed())
                active_field.active_pokes[effect_target]->remove_type(1);

            return Attack_Result::HIT;
        case MOVE_EFFECTS::FLAT_DAMAGE:
            int damage;
            if(move_effect.use_flat_level())
            {
                damage = active_field.active_pokes[atk_pos]->get_level();
            }
            else
            {
                damage = move_effect.get_flat_damage();
            }

            if(!active_field.active_pokes[effect_target]->deal_damage(damage))
            {
                handle_faint(effect_target);
                return Attack_Result::FAINT;
            }
            else
            {
                return Attack_Result::HIT;
            }
        case MOVE_EFFECTS::PROTECT:
            active_field.active_pokes[effect_target]->increment_protect_turns();
            if(roll_chance(1.0/active_field.active_pokes[effect_target]->get_protect_turns()))
            {
                active_field.active_pokes[effect_target]->protect_poke();
                return Attack_Result::HIT;
            }
            else
            {
                DEBUG_MSG("But it failed\n");
                return Attack_Result::MISS;
            }
        case MOVE_EFFECTS::SUBSTITUTE:
            if(active_field.active_pokes[effect_target]->setup_substitute())
                return Attack_Result::HIT;
            else
            {
                DEBUG_MSG("But it failed\n");
                return Attack_Result::NO_ATTACK;
            }
        default:
            ERR_MSG("Unhandled move effect " << move_effect.get_effect() << "\n");
    }
    ERR_MSG("Handling of move effect " << move_effect.get_effect() << " did not return correctly\n");
    return Attack_Result::NO_ATTACK;
}

bool Battle::has_lost(Players player)
{
    for(int i = 0; i < 6; i++)
    {
        if(Parties[player].party_pokes[i].is_alive())
        {
            return false;
        }
    }

    return true;
}

bool Battle::can_mega(FIELD_POSITION pos)
{
    if(active_field.active_pokes[pos] == nullptr)
        return false;
    return (has_megad[get_player_from_position(pos)] == inactive) && active_field.active_pokes[pos]->can_mega();
};

void Battle::mega_pending(FIELD_POSITION pos)
{
    if(has_megad[get_player_from_position(pos)] == inactive)
        has_megad[get_player_from_position(pos)] = pending;
    else
        ERR_MSG("Should not try to queue a mega evolution when there is already a mega evolution is pending");
}


void Battle::mega_evolve(FIELD_POSITION pos)
{
    if(!active_field.active_pokes[pos]->can_mega())
        ERR_MSG(active_field.active_pokes[pos]->get_species() << " cannot mega evolve" << endl);
    else
    {
        active_field.active_pokes[pos]->mega_evolve();
        has_megad[get_player_from_position(pos)] = active;
    }
}

bool Battle::can_swap(Players player)
{
    for(int i = 0; i < 6; i++)
    {
        if(Parties[player].party_pokes[i].is_alive() && !Parties[player].party_pokes[i].is_active())
        {
            DEBUG_MSG("Player " << (player + 1) << " can swap\n");
            return true;
        }
    }
    DEBUG_MSG("Player " << (player + 1) << " cannot swap\n");
    return false;
}

void Battle::reset_temp_field_status()
{
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        if(active_field.active_pokes[i] == nullptr)
            continue;

        active_field.active_pokes[i]->reset_types();
        active_field.active_pokes[i]->clear_volatile_status_mask(turn_end_v_status_mask_clear);

        //TODO: DOESNT WORK IN DOUBLES CORRECTLY DUE TO WIDEGAURD AND THE LIKE
        if(!active_field.active_pokes[i]->is_protected())
            active_field.active_pokes[i]->clear_protect_turns();
        active_field.active_pokes[i]->reset_protect();

#if BATTLE_TYPE != SINGLE_BATTLE
        //WARN_MSG("Protect clearing is only properly supported in single battles due to team wide protects\n");
#endif
    }
}

int Battle::calculate_damage_dealt(int attacker_level, int move_power, int atk, int def, vector<float>* damage_mods)
{
    float damage_adjustment = damage_calc(generator) / 100.0;
    damage_mods->insert(damage_mods->begin(), damage_adjustment);
    int base_damage  = floor(floor((floor((2 * attacker_level) / 5 + 2) * move_power * atk) / def) / 50 + 2);

    return do_chain_mult(base_damage, damage_mods);
}

void Battle::handle_faint(FIELD_POSITION pos)
{
    active_field.active_pokes[pos]->faint_poke();
    DEBUG_MSG("P" << (get_player_from_position(pos) + 1) << "'s "
           << active_field.active_pokes[pos]->get_species() << " FAINTED" << "\n");
}

Attack_Result Battle::handle_pre_attack_status(FIELD_POSITION pos)
{
    switch(active_field.active_pokes[pos]->get_status())
    {
        case STATUS::ASLEEP:
            DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is asleep\n");
            switch(active_field.active_pokes[pos]->status_turns)
            {
                case 0:
                    active_field.active_pokes[pos]->status_turns++;
                    return Attack_Result::NO_ATTACK;
                case 1:
                case 2:
                    if(roll_chance((float)2/3))
                    {
                        active_field.active_pokes[pos]->status_turns++;
                        return Attack_Result::NO_ATTACK;
                    }
                    else
                    {
                        DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " woke up!\n");
                        active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
                    }
                    break;
                case 3:
                    DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " woke up!\n");
                    active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
                    break;
                default:
                    ERR_MSG("Invliad Sleep Turns " << active_field.active_pokes[pos]->status_turns << "\n");
            }
            return Attack_Result::HIT;
        case STATUS::PARALYZED:
            DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is paralyzed and may not be able to attack\n");
            if(roll_chance(0.25))
            {
                DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is paralyzed\n");
                return Attack_Result::NO_ATTACK;
            }
            return Attack_Result::HIT;
        case STATUS::FROZEN:
            if(roll_chance(0.8))
            {
                DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is frozen\n");
                return Attack_Result::NO_ATTACK;
            }

            DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " thawed!\n");
            active_field.active_pokes[pos]->set_status(STATUS::NO_STATUS);
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
        if(active_field.active_pokes[i] == nullptr)
            continue;
        else if(!handle_end_turn_statuses(static_cast<FIELD_POSITION>(i)))
        {
            handle_faint(static_cast<FIELD_POSITION>(i));
            if(has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
                return true;
            fainted = true;
        }
    }

    return fainted;
}

bool Battle::handle_end_turn_statuses(FIELD_POSITION pos)
{
    if(!active_field.position_alive(pos))
        return true;

    if(handle_end_turn_status(pos)
    && !(handle_v_status_mask(pos, turn_end_v_status_mask) == Attack_Result::FAINT)
    && active_field.handle_end_turn_field_obj(pos))
        return true;
    else
    {
        handle_faint(pos);
        return false;
    }
}

bool Battle::handle_end_turn_status(FIELD_POSITION pos)
{
    float damage = 0;
    if(active_field.active_pokes[pos]->get_status() != STATUS::NO_STATUS
    && active_field.active_pokes[pos]->get_status() != STATUS::PARALYZED)
        DEBUG_MSG("P" << get_player_from_position(pos) + 1 << "'s " << active_field.active_pokes[pos]->get_species() << " is");

    switch(active_field.active_pokes[pos]->get_status())
    {
        case STATUS::BURNED:
            DEBUG_MSG(" burned\n");
            DEBUG_MSG("Burn ");
            damage = active_field.active_pokes[pos]->get_stat(STAT::HP) / 16.0;
            break;
        case STATUS::POISONED:
            DEBUG_MSG(" poisoned\n");
            DEBUG_MSG( "Poison ");
            damage = active_field.active_pokes[pos]->get_stat(STAT::HP) / 8.0;
            break;
        case STATUS::BADLY_POISONED:
            DEBUG_MSG(" badly poisoned\n");
            DEBUG_MSG("Poison ");
            active_field.active_pokes[pos]->status_turns++;
            damage = active_field.active_pokes[pos]->get_stat(STAT::HP) / 16.0 * active_field.active_pokes[pos]->status_turns;
            break;
        default:
            return true;
    }
    return active_field.active_pokes[pos]->deal_damage(damage);
}

Attack_Result Battle::handle_v_status_mask(FIELD_POSITION pos, int status_mask, int move_num)
{
    int current_v_status;
    Attack_Result res = Attack_Result::HIT;
    for(int i = 0; i < VOLATILE_STATUS_NUMBERS::NUM_VOLATILE_STATUS; i++)
    {
        current_v_status = (1u << i);
        if((current_v_status & status_mask) && (current_v_status & active_field.active_pokes[pos]->get_volatile_status()))
        {
            res = handle_v_status(pos, current_v_status, move_num);
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
            DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is confused\n");
            switch(active_field.active_pokes[pos]->get_v_status_turns(NUM_CONFUSION))
            {
                case 0:
                    active_field.active_pokes[pos]->increment_v_status_turns(NUM_CONFUSION);
                    break;
                case 1:
                case 2:
                case 3:
                    if(roll_chance(0.25))
                    {
                        active_field.active_pokes[pos]->clear_v_status_turns(NUM_CONFUSION);
                        active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::CONFUSION);
                        DEBUG_MSG( active_field.active_pokes[pos]->get_species() << " snapped out of its confusion\n");
                        return Attack_Result::HIT;
                    }
                    active_field.active_pokes[pos]->increment_v_status_turns(NUM_CONFUSION);
                    break;
                case 4:
                    active_field.active_pokes[pos]->clear_v_status_turns(NUM_CONFUSION);
                    active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::CONFUSION);
                    DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " snapped out of its confusion\n");
                    return Attack_Result::HIT;
                default:
                    ERR_MSG("Confusion turn count impossible\n");
            }

            if(roll_chance(game_moves[Game_Moves::MOVE_CONFUSION].get_acc()))
            {
                DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " hurt itself in its confusion\n");
                if(attack_damage(pos, pos, &game_moves[Game_Moves::MOVE_CONFUSION]).first == Attack_Result::FAINT)
                    return Attack_Result::FAINT;
                else
                    return Attack_Result::NO_ATTACK;
            }

            return Attack_Result::HIT;
        case VOLATILE_STATUS::TAUNTED:
            if(active_field.active_pokes[pos]->get_v_status_turns(NUM_TAUNTED) < 4)
            {
                active_field.active_pokes[pos]->increment_v_status_turns(NUM_TAUNTED);
                if(active_field.active_pokes[pos]->moves[move_num].get_damage_type() == move_damage_type::MOVE_STATUS)
                {
                    DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is taunted and can't use "
                           << active_field.active_pokes[pos]->moves[move_num].get_name() << endl);
                    return Attack_Result::NO_ATTACK;
                }
                return Attack_Result::HIT;
            }
            else if(active_field.active_pokes[pos]->get_v_status_turns(NUM_TAUNTED) == 4)
            {
                DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " is no longer taunted\n");
                active_field.active_pokes[pos]->clear_v_status_turns(NUM_TAUNTED);
                active_field.active_pokes[pos]->clear_volatile_status(VOLATILE_STATUS::TAUNTED);
                return Attack_Result::HIT;
            }
            ERR_MSG("Impossible Taunt status handling\n");
        case VOLATILE_STATUS::FLINCHED:
            DEBUG_MSG(active_field.active_pokes[pos]->get_species() << " flinched\n");
            return Attack_Result::NO_ATTACK;
        case VOLATILE_STATUS::BOUND:
        case VOLATILE_STATUS::CANT_ESCAPE:
        case VOLATILE_STATUS::CURSE:
        case VOLATILE_STATUS::EMBARGO:
        case VOLATILE_STATUS::ENCORE:
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
        return roll_chance(acc*atk_acc_mod*(1/def_eva_mod));
    }
}

bool Battle::roll_chance(float pcent_chance)
{
    float c = chance(generator);
    return c < pcent_chance;
}

void Battle::calculate_damage_modifier(vector<float>* mults, Move* move, Pokemon* attacker, Pokemon* defender, int num_targets, bool crit)
{
    if(is_stab(attacker->get_type(), move->get_type()))
        mults->push_back(1.5);

    if(crit)
    {
        DEBUG_MSG("Critical Hit\n");
        mults->push_back(1.5);
    }

    if(num_targets > 1)
        mults->push_back(0.75);

    if((active_field.weather_state == Weather::RAIN && move->get_type() == PokeTypes::WATER)
    || (active_field.weather_state == Weather::HEAVY_RAIN && move->get_type() == PokeTypes::WATER)
    || (active_field.weather_state == Weather::HARSH_SUNLIGHT && move->get_type() == PokeTypes::FIRE)
    || (active_field.weather_state == Weather::EXTREMELY_HARSH_SUNLIGHT && move->get_type() == PokeTypes::FIRE))
        mults->push_back(1.5);

    if((active_field.weather_state == Weather::RAIN && move->get_type() == PokeTypes::FIRE)
    || (active_field.weather_state == Weather::HARSH_SUNLIGHT && move->get_type() == PokeTypes::WATER))
        mults->push_back(0.5);

    if((active_field.weather_state == Weather::HEAVY_RAIN && move->get_type() == PokeTypes::FIRE)
    || (active_field.weather_state == Weather::EXTREMELY_HARSH_SUNLIGHT && move->get_type() == PokeTypes::WATER))
        mults->push_back(0);

    if(move->get_damage_type() == move_damage_type::MOVE_PHYSICAL && attacker->get_status() == STATUS::BURNED)
        mults->push_back(0.5);

    if(move->get_type() == PokeTypes::GROUND && !defender->is_grounded())
        mults->push_back(0);

    mults->push_back(calculate_type_damage_modifier(defender->get_type(), move->get_type()));

    if(mults->back() >= 2)
        DEBUG_MSG("It's super effective\n");
    else if(mults->back() > 0 && mults->back() < 1)
        DEBUG_MSG("It's not very effective\n");
}

// LOADING AND RESETTING
void Battle::reset()
{
    active_field.reset();
    Parties[Players::PLAYER_ONE].reset();
    Parties[Players::PLAYER_TWO].reset();
    has_megad[PLAYER_ONE] = inactive;
    has_megad[PLAYER_TWO] = inactive;
}

void Battle::update_generator(long seed)
{
    generator = mt19937(seed);
}

void Battle::load_battle(Players player, string* team_name)
{
    load_teams(player, *team_name);
    load_game_moves();
    has_megad[PLAYER_ONE] = inactive;
    has_megad[PLAYER_TWO] = inactive;
}

int Battle::count_pokemon_team(boost::property_tree::ptree team)
{
    int num_pokemon = 0;
    for(int i = 0; i < 6; i++)
    {
        if(team.count(to_string(i)))
            num_pokemon++;
        else
            return num_pokemon;
    }
    return num_pokemon;
}

void Battle::load_teams(Players player, string team_name)
{
    boost::property_tree::ptree team = load_json_file("teams/" + team_name + ".json");
    int num_pokemon = count_pokemon_team(team), poke_loading;

    for(int i = 0; i < 6; i++)
    {
        if(i < num_pokemon)
            poke_loading = i;
        else
            poke_loading = num_pokemon - 1;

        Parties[player].party_pokes[i].load_pokemon(team.get_child(to_string(poke_loading)));
        DEBUG_MSG("Finished importing pokemon " << i + 1 << endl << endl);
    }
    Parties[player].loaded = true;
    DEBUG_MSG("Finished importing Player "  << player+1 << "'s team: " << team_name << endl);
}

void Battle::load_game_moves()
{
    game_moves[Game_Moves::MOVE_CONFUSION].load_move("game_moves/Confusion");
    game_moves[Game_Moves::MOVE_STRUGGLE].load_move("game_moves/Struggle");
}

#ifdef DEBUGGING
void Battle::print_battle()
{
    active_field.print_field();

    DEBUG_MSG("Player ONE party pokemon:\n");
    Parties[Players::PLAYER_ONE].print_party();
    DEBUG_MSG("\nPlayer TWO party pokemon:\n");
    Parties[Players::PLAYER_TWO].print_party();
    DEBUG_MSG(endl);
}
#else
void Battle::print_battle() {}
#endif