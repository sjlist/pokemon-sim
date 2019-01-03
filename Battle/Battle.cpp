//
// Created by slist on 12/13/18.
//

#include "Battle/Battle.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Type.h"
#include "Battle/Field.h"

#include "fileIO/loadJSON.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

Battle::Battle()
{

}

Party::Party()
{

}

Party Battle::get_party(Players player)
{
    return Parties[player];
}

void Battle::send_out(FIELD_POSITION pos, int poke_position)
{
    Players player = get_player_from_position(pos);
    std::cout << "Sending out P" << player + 1 << "'s " << Parties[player].party_pokes[poke_position].get_species() << "\n";
    //Battle::print_battle(true);
    if(Parties[player].party_pokes[poke_position].get_status() == STATUS::BADLY_POISONED)
        Parties[player].party_pokes[poke_position].status_turns = 0;

    Battle::active_field.send_out(pos, Parties[player].party_pokes[poke_position]);
    Parties[player].party_pokes[poke_position].set_active(true);

}

void Battle::return_poke(FIELD_POSITION pos)
{
    for(int i = 0; i < 6; i++)
    {
        if(Battle::active_field.active_pokes[pos].get_species() == Battle::Parties[get_player_from_position(pos)].party_pokes[i].get_species())
        {
            Battle::Parties[get_player_from_position(pos)].party_pokes[i].set_active(false);
            Battle::Parties[get_player_from_position(pos)].party_pokes[i].clear_stat_mods();
            Battle::Parties[get_player_from_position(pos)].party_pokes[i].clear_volitile_statuses();
            Battle::active_field.return_poke(pos);
            break;;
        }
    }
};

void Battle::swap_poke(FIELD_POSITION pos, int poke_position)
{
    Battle::return_poke(pos);
    Battle::send_out(pos, poke_position);
}

Attack_Result Battle::attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move_number)
{
    Attack_Result res;
    bool crit = false;

    if(!Battle::handle_pre_attack_status(atk_pos))
        return Attack_Result::NO_ATTACK;

    if(!Battle::handle_pre_attack_v_statuses(atk_pos))
        return Attack_Result::NO_ATTACK;

    // Check if there is enough pp to use the move
    if(!Battle::active_field.active_pokes[atk_pos].use_move(move_number))
    {
        std::cout << "Not enough PP\n";
        return Attack_Result::NO_PP;
    }

    std::cout << "P" << get_player_from_position(atk_pos) + 1  << "'s " << Battle::active_field.active_pokes[atk_pos].get_species() << " used "
              << Battle::active_field.active_pokes[atk_pos].moves[move_number].get_name() << "\n";

    // roll for hit
    if(!Battle::roll_acc(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_acc(),
                         Battle::active_field.active_pokes[atk_pos].get_stat(STAT::ACC),
                         Battle::active_field.active_pokes[def_pos].get_stat(STAT::EVA)))
    {
        std::cout << Battle::active_field.active_pokes[atk_pos].moves[move_number].get_name() << " missed\n";
        return Attack_Result::MISS;
    }

    if(Battle::roll_chance(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_crit()))
        crit = true;

    if(def_pos == FIELD_POSITION::ALL_TARGETS)
    {
        Battle::Battle_Targets.get_valid_targets(
                Battle::active_field.active_pokes[atk_pos].moves[move_number].get_move_targets(), atk_pos);
        for(int i = 0; i < Battle::active_field.active_pokes[atk_pos].moves[move_number].get_num_targets(); i++)
        {
            res = Battle::attack_target(atk_pos, Battle::Battle_Targets.valid_targets[i], Battle::active_field.active_pokes[atk_pos].moves[move_number], crit);
        }
    }
    else
        res = Battle::attack_target(atk_pos, def_pos, Battle::active_field.active_pokes[atk_pos].moves[move_number], crit);

    return res;
}

Attack_Result Battle::attack_target(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move move, bool crit)
{
    Attack_Result res;
    switch(move.get_damage_type())
    {
        case move_damage_type::MOVE_PHYSICAL:
        case move_damage_type::MOVE_SPECIAL:
            res = Battle::attack_damage(atk_pos, def_pos, move, crit);
            if(res == Attack_Result::IMMUNE)
                return res;
            break;;
        case move_damage_type::MOVE_STATUS:
            res = Attack_Result::NO_ATTACK;
            break;
        default:
            std::cout << "Unhandled attack type" << move.get_damage_type() << "\n";
            assert(0);
    }

    // Handling move effects
    for(int i = 0; i < MAX_EFFECTS; i++)
    {
        if (move.get_move_effect(i).get_effect() != NO_MOVE_EFFECT)
        {
            res = Battle::handle_move_effects(
                    move.get_move_effect(i), atk_pos, def_pos);
            if (res == Attack_Result::SWAP)
                return res;
        }
        else
            break;
    }

    // Handle returning faint if needed
    if(!Battle::active_field.active_pokes[def_pos].is_alive())
        return Attack_Result::FAINT;
}

Attack_Result Battle::attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move move, bool crit)
{
    int eff_atk, eff_def, damage_dealt;
    float damage_mod;

    damage_mod = Battle::calculate_damage_modifier(move, Battle::active_field, Battle::active_field.active_pokes[atk_pos], Battle::active_field.active_pokes[def_pos], move.get_num_targets(), crit);

    if(damage_mod == 0)
    {
        std::cout << "P" << get_player_from_position(def_pos) + 1  << "'s " << Battle::active_field.active_pokes[def_pos].get_species()
                  << " is immune to " << type_to_string(move.get_type()) << " type moves\n";
        return Attack_Result::IMMUNE;
    }

    // determine the effective stats to use
    if(move.get_damage_type() == move_damage_type::MOVE_PHYSICAL)
    {
        eff_atk = Battle::active_field.active_pokes[atk_pos].get_stat(STAT::ATK);
        eff_def = Battle::active_field.active_pokes[def_pos].get_stat(STAT::DEF);
    }
    else
    {
        eff_atk = Battle::active_field.active_pokes[atk_pos].get_stat(STAT::SPA);
        eff_def = Battle::active_field.active_pokes[def_pos].get_stat(STAT::SPD);
    }

    // calculate damage dealt
    damage_dealt = Battle::calculate_damage_dealt(Battle::active_field.active_pokes[atk_pos].get_level(), move.get_power(), eff_atk, eff_def, damage_mod);

    // Deal damage and handle fainting but DO NOT RETURN FAINT RESULT UNTIL AFTER SWAP HAS A CHANCE TO RETURN
    if(!Battle::active_field.active_pokes[def_pos].deal_damage(damage_dealt))
    {
        Battle::handle_faint(def_pos);
        return Attack_Result::FAINT;
    }

    return Attack_Result::HIT;
}

Attack_Result Battle::handle_move_effects(Effect move_effect, FIELD_POSITION atk_pos, FIELD_POSITION def_pos)
{
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
        case MOVE_EFFECTS::NON_VOLITILE_STATUS_EFFECT:
            if(Battle::roll_chance(move_effect.get_effect_chance()))
            {
                if(Battle::active_field.active_pokes[def_pos].set_status(move_effect.get_effect_status_type()))
                    std::cout << "P" << get_player_from_position(def_pos) + 1 << "'s "
                              << Battle::active_field.active_pokes[def_pos].get_species()
                              << " is now " << status_to_string(move_effect.get_effect_status_type()) << "\n";
            }
            return Attack_Result::HIT;
        case MOVE_EFFECTS::VOLITILE_STATUS_EFFECT:
            if(Battle::roll_chance(move_effect.get_effect_chance()))
            {
                if(Battle::active_field.active_pokes[def_pos].set_volitile_status(move_effect.get_volitile_status_effect()))
                {
                    std::cout << "P" << get_player_from_position(def_pos) + 1 << "'s "
                              << Battle::active_field.active_pokes[def_pos].get_species()
                              << " is now " << v_status_to_string(move_effect.get_volitile_status_effect()) << "\n";
                }
                return Attack_Result::HIT;
            }
            return Attack_Result::HIT;
        case MOVE_EFFECTS::STAT_CHANGE:
            Battle::active_field.active_pokes[atk_pos].stat_change(move_effect.get_stat_changed(), move_effect.get_stages_changed());
            return Attack_Result::HIT;
        default:
            std::cout << "Unhandled move effect " << move_effect.get_effect() << "\n";
            assert(0);
    }
}

bool Battle::has_lost(Players player)
{
    if(Battle::active_field.active_pokes[player].is_alive())
    {
        return false;
    }

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

void Battle::update_party(Players player)
{
    for(int i = 0; i < 6; i++)
    {
        if(Battle::active_field.active_pokes[player].get_species() == Battle::Parties[player].party_pokes[i].get_species())
        {
            Battle::Parties[player].party_pokes[i] = Battle::active_field.active_pokes[player];
            return;
        }
    }
}

int Battle::calculate_damage_dealt(int attacker_level, int move_power, int atk, int def, float damage_modifier)
{
    int base_damage = ((((2 * attacker_level / 5) + 2) * move_power * atk / def / 50) + 2) * damage_modifier;
    float damage_adjustment = (rand()/(float)RAND_MAX * 0.15) + 0.85;

    return (float)base_damage * damage_adjustment;
}

void Battle::handle_faint(FIELD_POSITION pos)
{
    Battle::active_field.active_pokes[pos].faint_poke();
    Battle::update_party(get_player_from_position(pos));
    std::cout << "P" << (get_player_from_position(pos) + 1) << "'s " << Battle::active_field.active_pokes[pos].get_species() << " FAINTED" << "\n";
}

bool Battle::handle_pre_attack_status(FIELD_POSITION pos)
{
    switch(Battle::active_field.active_pokes[pos].get_status())
    {
        case STATUS::ASLEEP:
            if(Battle::active_field.active_pokes[pos].status_turns == 0)
            {
                Battle::active_field.active_pokes[pos].status_turns++;
                return false;
            }
            else if(Battle::active_field.active_pokes[pos].status_turns < 3)
            {
                if(Battle::roll_chance((float)2/3))
                {
                    Battle::active_field.active_pokes[pos].status_turns++;
                    return false;
                }
                else
                {
                    std::cout << Battle::active_field.active_pokes[pos].get_species() << " woke up!\n";
                    Battle::active_field.active_pokes[pos].set_status(STATUS::NO_STATUS);
                }
            }
            else
            {
                std::cout << Battle::active_field.active_pokes[pos].get_species() << " woke up!\n";
                Battle::active_field.active_pokes[pos].set_status(STATUS::NO_STATUS);
            }
            return true;
        case STATUS::PARALYZED:
            if(Battle::roll_chance(0.25))
            {
                std::cout << Battle::active_field.active_pokes[pos].get_species() << " is paralyzed\n";
                return false;
            }

            return true;
        case STATUS::FROZEN:
            if(Battle::roll_chance(0.8))
            {
                std::cout << Battle::active_field.active_pokes[pos].get_species() << " is frozen\n";
                return false;
            }

            std::cout << Battle::active_field.active_pokes[pos].get_species() << " thawed!\n";
            Battle::active_field.active_pokes[pos].set_status(STATUS::NO_STATUS);
            return true;
        default:
            return true;
    }
}

bool Battle::handle_end_turn_field_status()
{
    bool fainted = false;
    if(!Battle::handle_end_turn_status(FIELD_POSITION::PLAYER_1_0))
    {
        Battle::handle_faint(FIELD_POSITION::PLAYER_1_0);
        fainted = true;
    }

    if(!Battle::handle_end_turn_status(FIELD_POSITION::PLAYER_2_0))
    {
        Battle::handle_faint(FIELD_POSITION::PLAYER_2_0);
        fainted = true;
    }

    return fainted;
}

bool Battle::handle_end_turn_status(FIELD_POSITION pos)
{
    int damage = 0;
    switch(Battle::active_field.active_pokes[pos].get_status())
    {
        case STATUS::BURNED:
            std::cout << "Burn ";
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 8;
            break;;
        case STATUS::POISONED:
            std::cout << "Poison ";
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 8;
            break;;
        case STATUS::BADLY_POISONED:
            std::cout << "Poison ";
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 16 * Battle::active_field.active_pokes[pos].status_turns;
            break;;
        default:
            return true;
    }
    return Battle::active_field.active_pokes[pos].deal_damage(damage);
}

bool Battle::handle_pre_attack_v_statuses(FIELD_POSITION pos)
{
    int temp_v_status = Battle::active_field.active_pokes[pos].get_volitile_status();
    int current_v_status, i = 0;
    bool can_attack = true;
    while(temp_v_status > 0)
    {
        current_v_status = (1u << i);
        if(current_v_status & temp_v_status)
        {
            temp_v_status &= ~(current_v_status);
            if(!Battle::handle_pre_attack_v_status(pos, current_v_status))
                can_attack = false;
        }
        i++;
    }
    return can_attack;
}

bool Battle::handle_pre_attack_v_status(FIELD_POSITION pos, int v_status)
{
    switch(v_status)
    {
        case VOLITILE_STATUS::CONFUSION:
            std::cout << Battle::active_field.active_pokes[pos].get_species() << " is confused\n";
            switch(Battle::status_turns[NUM_CONFUSION])
            {
                case 0:
                    Battle::status_turns[NUM_CONFUSION]++;
                    break;;
                case 1:
                case 2:
                case 3:
                    if(Battle::roll_chance(0.25))
                    {
                        Battle::status_turns[NUM_CONFUSION] = 0;
                        Battle::active_field.active_pokes[pos].clear_volitile_status(VOLITILE_STATUS::CONFUSION);
                        std::cout << Battle::active_field.active_pokes[pos].get_species() << " snapped out of its confusion\n";
                        return true;
                    }
                    Battle::status_turns[NUM_CONFUSION]++;
                    break;;
                case 4:
                    Battle::status_turns[NUM_CONFUSION] = 0;
                    Battle::active_field.active_pokes[pos].clear_volitile_status(VOLITILE_STATUS::CONFUSION);
                    std::cout << Battle::active_field.active_pokes[pos].get_species() << " snapped out of its confusion\n";
                    return true;
                default:
                    assert(0);
            }

            if(roll_chance(status_moves[NUM_CONFUSION].get_acc()))
            {
                std::cout << Battle::active_field.active_pokes[pos].get_species() << " hurt itself in its confusion\n";
                Battle::attack_damage(pos, pos, status_moves[NUM_CONFUSION], false);
                return false;
            }

            return true;
        case VOLITILE_STATUS::BOUND:
        case VOLITILE_STATUS::CANT_ESCAPE:
        case VOLITILE_STATUS::CURSE:
        case VOLITILE_STATUS::EMBARGO:
        case VOLITILE_STATUS::ENCORE:
        case VOLITILE_STATUS::FLINCHED:
        case VOLITILE_STATUS::HEALBLOCK:
        case VOLITILE_STATUS::IDENTIFIED:
        case VOLITILE_STATUS::INFATUATION:
        case VOLITILE_STATUS::NIGHTMARE:
        case VOLITILE_STATUS::PERISHSONG:
        case VOLITILE_STATUS::TAUNT:
        case VOLITILE_STATUS::TELEKINESIS:
        case VOLITILE_STATUS::TORMENT:
            assert(0);
    }
    return true;
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
    float  c = rand()/(float)RAND_MAX;
    return c < chance;
}

float Battle::calculate_damage_modifier(Move move, Field field, Pokemon attacker, Pokemon defender, int num_targets, bool crit)
{
    float damage_modifier = 1;

    if(num_targets > 1)
        damage_modifier *= 0.75;

    if((field.weather_state == Weather::RAIN && move.get_type() == PokeTypes::WATER) || (field.weather_state == Weather::HARSH_SUNLIGHT && move.get_type() == PokeTypes::FIRE))
        damage_modifier *= 1.5;

    if((field.weather_state == Weather::RAIN && move.get_type() == PokeTypes::FIRE) || (field.weather_state == Weather::HARSH_SUNLIGHT && move.get_type() == PokeTypes::WATER))
        damage_modifier *= 0.5;

    if(is_stab(attacker.get_type(), move.get_type()))
        damage_modifier *= 1.5;

    if(move.get_damage_type() == move_damage_type::MOVE_PHYSICAL && attacker.get_status() == STATUS::BURNED)
        damage_modifier *= 0.5;

    damage_modifier *= calculate_type_damage_modifier(defender.get_type(), move.get_type());

    return damage_modifier;
}

void Battle::load_battle()
{
    Battle::load_teams(Battle::select_teams());
    Battle::load_status_moves();
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
                std::cout << "Finished importing " << team_names[Players::PLAYER_ONE] << "\n";
                done[Players::PLAYER_ONE] = true;
            }
        }
        if(!done[1])
        {
            try
            {
                Parties[Players::PLAYER_TWO].party_pokes[i].load_pokemon(team_2.get_child(std::to_string(i)));
            }
            catch(...)
            {
                std::cout << "Finished importing " << team_names[Players::PLAYER_TWO] << "\n";
                done[Players::PLAYER_TWO] = true;
            }
        }
        i++;
    }
}

std::vector<std::string> Battle::select_teams()
{
    std::vector<std::string> teams(2);
    teams[Players::PLAYER_ONE] = "team1";
    teams[Players::PLAYER_TWO] = "team2";
    return teams;
}

void Battle::load_status_moves()
{
    Battle::status_moves[NUM_CONFUSION].load_move("status_moves/Confusion");
}

void Battle::print_battle(bool detailed)
{
    Battle::active_field.print_field(detailed);
    if(detailed)
    {
        std::cout << "Player ONE party pokemon:\n";
        for (int i = 0; i < 6; i++) {
            if (!Battle::Parties[PLAYER_ONE].party_pokes[i].is_active())
                Battle::Parties[PLAYER_ONE].party_pokes[i].print_pokemon();
        }
        std::cout << "\nPlayer TWO party pokemon:\n";
        for (int i = 0; i < 6; i++) {
            if (!Battle::Parties[PLAYER_TWO].party_pokes[i].is_active())
                Battle::Parties[PLAYER_TWO].party_pokes[i].print_pokemon();
        }
    }
}