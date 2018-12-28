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

void Battle::load_battle()
{
    Battle::load_teams(Battle::select_teams());
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
    int eff_atk, eff_def, damage_dealt;
    float damage_mod;

    // Check if there is enough pp to use the move
    if(!Battle::active_field.active_pokes[atk_pos].use_move(move_number))
    {
        std::cout << "Not enough PP\n";
        return Attack_Result::NO_PP;
    }

    if(!Battle::handle_pre_attack_status(atk_pos))
        return Attack_Result::NO_ATTACK;

    std::cout << "P" << get_player_from_position(atk_pos) + 1  << "'s " << Battle::active_field.active_pokes[atk_pos].get_species() << " is attacking "
              << "P" << get_player_from_position(def_pos) + 1  << "'s " << Battle::active_field.active_pokes[def_pos].get_species() << " with "
              << Battle::active_field.active_pokes[atk_pos].moves[move_number].get_name() << "\n";

    // roll for hit
    if(!Battle::roll_acc(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_acc()))
    {
        std::cout << Battle::active_field.active_pokes[atk_pos].moves[move_number].get_name() << " missed\n";
        return Attack_Result::MISS;
    }


    damage_mod = Battle::calculate_damage_modifier(Battle::active_field.active_pokes[atk_pos].moves[move_number], Battle::active_field, Battle::active_field.active_pokes[atk_pos], Battle::active_field.active_pokes[def_pos], 1);

    if(damage_mod == 0)
    {
        std::cout << "P" << get_player_from_position(def_pos) + 1  << "'s " << Battle::active_field.active_pokes[def_pos].get_species()
                  << " is immune to " << type_to_string(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_type()) << " type moves\n";
        return Attack_Result::IMMUNE;
    }

    // determine the effective stats to use
    if(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_damage_type() == "physical")
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
    damage_dealt = Battle::calculate_damage_dealt(Battle::active_field.active_pokes[atk_pos].get_level(), Battle::active_field.active_pokes[atk_pos].moves[move_number].get_power(), eff_atk, eff_def, damage_mod);

    // Deal damage and handle fainting but DO NOT RETURN FAINT RESULT UNTIL AFTER SWAP HAS A CHANCE TO RETURN
    if(!Battle::active_field.active_pokes[def_pos].deal_damage(damage_dealt))
    {
        Battle::handle_faint(def_pos);
    }

    // if attack is a swapping attack
    if(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_move_effect() == MOVE_EFFECTS::SWAP)
    {
        return Attack_Result::SWAP;
    }

    // Handle returning faint if needed
    if(!Battle::active_field.active_pokes[def_pos].is_alive())
        return Attack_Result::FAINT;

    // Handling move effects
    if(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_move_effect() != MOVE_EFFECTS::NO_MOVE_EFFECT && Battle::roll_acc(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_status_chance()))
    {
        Attack_Result move_result = Battle::handle_move_effects(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_move_effect(), def_pos);
        if(move_result != Attack_Result::NO_ATTACK)
            return move_result;
    }

    // handle status effects
    if((Battle::active_field.active_pokes[atk_pos].moves[move_number].get_status_effect() != STATUS::NO_STATUS) && Battle::roll_acc(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_status_chance()))
    {
        std::cout << "P" << get_player_from_position(def_pos) + 1 << "'s " << Battle::active_field.active_pokes[def_pos].get_species()
                  << " is now " << status_to_string(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_status_effect()) << "\n";
        Battle::active_field.active_pokes[def_pos].set_status(Battle::active_field.active_pokes[atk_pos].moves[move_number].get_status_effect());
    }

    return Attack_Result::HIT;
}

void Battle::handle_faint(FIELD_POSITION pos)
{
    Battle::active_field.active_pokes[pos].faint_poke();
    Battle::update_party(get_player_from_position(pos));
    std::cout << "P" << (get_player_from_position(pos) + 1) << "'s " << Battle::active_field.active_pokes[pos].get_species() << " FAINTED" << "\n";
}

Attack_Result Battle::handle_move_effects(MOVE_EFFECTS move_effect, FIELD_POSITION def_pos)
{
    switch(move_effect)
    {
        case MOVE_EFFECTS::FLINCH:
            std::cout << "P" << get_player_from_position(def_pos) + 1  << "'s " << Battle::active_field.active_pokes[def_pos].get_species() << " flinched\n";
            return Attack_Result::FLINCHED;
        default:
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
    if(Battle::handle_end_turn_status(FIELD_POSITION::PLAYER_1_0))
    {
        Battle::handle_faint(FIELD_POSITION::PLAYER_1_0);
        fainted = true;
    }

    if(Battle::handle_end_turn_status(FIELD_POSITION::PLAYER_2_0))
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
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 8;
            break;;
        case STATUS::POISONED:
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 8;
            break;;
        case STATUS::BADLY_POISONED:
            damage = (float)Battle::active_field.active_pokes[pos].get_stat(STAT::HP) / 16 * Battle::active_field.active_pokes[pos].status_turns;
            break;;
        default:
            return false;
    }
    return Battle::active_field.active_pokes[pos].deal_damage(damage);
}

bool Battle::roll_acc(float acc)
{
    return Battle::roll_chance(acc);
}

bool Battle::roll_chance(float chance)
{
    float  c = rand()/(float)RAND_MAX;
    return c < chance;
}

float Battle::calculate_damage_modifier(Move move, Field field, Pokemon attacker, Pokemon defender, int num_targets)
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

    if(move.get_damage_type() == "physical" && attacker.get_status() == STATUS::BURNED)
        damage_modifier *= 0.5;

    damage_modifier *= calculate_type_damage_modifier(defender.get_type(), move.get_type());

    return damage_modifier;
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