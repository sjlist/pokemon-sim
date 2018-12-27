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

void Battle::send_out(Players player, int poke_position)
{
    switch(player)
    {
        case Players::PLAYER_ONE:
            Battle::active_field.send_out(Players::PLAYER_ONE, Parties[Players::PLAYER_ONE].party_pokes[poke_position]);
            Parties[Players::PLAYER_ONE].party_pokes[poke_position].set_active(true);
            break;;
        case Players::PLAYER_TWO:
            Battle::active_field.send_out(Players::PLAYER_TWO, Parties[Players::PLAYER_TWO].party_pokes[poke_position]);
            Parties[Players::PLAYER_TWO].party_pokes[poke_position].set_active(true);
            break;;
        default :
            assert(0);
    }
}

Attack_Result Battle::attack(FIELD_POSITION pos, int move_number)
{
    int eff_atk, eff_def, damage_dealt;
    float damage_mod;

    if(!Battle::active_field.active_pokes[pos].use_move(move_number))
    {
        std::cout << "Not enough PP\n";
        return Attack_Result::NO_PP;
    }

    std::cout << Battle::active_field.active_pokes[pos].get_species() << " is attacking " << Battle::active_field.active_pokes[!pos].get_species() << " with " << Battle::active_field.active_pokes[pos].moves[move_number].get_name() << "\n";

    if(!Battle::roll_acc(Battle::active_field.active_pokes[pos].moves[move_number].get_acc()))
    {
        std::cout << Battle::active_field.active_pokes[pos].moves[move_number].get_name() << " missed\n";
        return Attack_Result::MISS;
    }

    if(Battle::active_field.active_pokes[pos].moves[move_number].get_damage_type() == "physical")
    {
        eff_atk = Battle::active_field.active_pokes[pos].get_stat(STAT::ATK);
        eff_def = Battle::active_field.active_pokes[!pos].get_stat(STAT::DEF);
    }
    else
    {
        eff_atk = Battle::active_field.active_pokes[pos].get_stat(STAT::SPA);
        eff_def = Battle::active_field.active_pokes[!pos].get_stat(STAT::SPD);
    }

    damage_mod = Battle::calculate_damage_modifier(Battle::active_field.active_pokes[pos].moves[move_number], Battle::active_field, Battle::active_field.active_pokes[pos], Battle::active_field.active_pokes[!pos], 1);
    damage_dealt = Battle::calculate_damaage_dealt(Battle::active_field.active_pokes[pos].get_level(), Battle::active_field.active_pokes[pos].moves[move_number].get_power(), eff_atk, eff_def, damage_mod);

    if(!Battle::active_field.active_pokes[!pos].deal_damage(damage_dealt))
    {
        Battle::active_field.active_pokes[!pos].faint_poke();
        Battle::update_party(get_player_from_position(!pos));
        std::cout << Battle::active_field.active_pokes[!pos].get_species() << " FAINTED" << "\n";
        return Attack_Result::FAINT;
    }

    return Attack_Result::HIT;

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

int Battle::calculate_damaage_dealt(int attacker_level, int move_power, int atk, int def, float damage_modifier)
{
    int base_damage = ((((2 * attacker_level / 5) + 2) * move_power * atk / def / 50) + 2) * damage_modifier;
    float damage_adjustment = (rand()/(float)RAND_MAX * 0.15) + 0.85;

    return (float)base_damage * damage_adjustment;
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

    damage_modifier *= calculate_type_damage_modifier(defender.get_type(), move.get_type());

    return damage_modifier;
}

void Battle::print_battle(bool detailed)
{
    Battle::active_field.print_field(detailed);
}