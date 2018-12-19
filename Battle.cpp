//
// Created by slist on 12/13/18.
//

#include "Battle.h"
#include "Pokemon.h"
#include "loadJSON.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

Battle::Battle()
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
                p1_party[i].load_pokemon(team_1.get_child(std::to_string(i)));
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
                p2_party[i].load_pokemon(team_2.get_child(std::to_string(i)));
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
            Battle::active_field.send_out(Players::PLAYER_ONE, p1_party[poke_position]);
            p1_party[poke_position].set_active(true);
            break;;
        case Players::PLAYER_TWO:
            Battle::active_field.send_out(Players::PLAYER_TWO, p2_party[poke_position]);
            p2_party[poke_position].set_active(true);
            break;;
        default :
            assert(0);
    }
}

// Battle init occurs after teams are loaded in
void Battle::init()
{
    std::srand(time(NULL));
    Battle::send_out(Players::PLAYER_ONE, 0);
    Battle::send_out(Players::PLAYER_TWO, 0);
    Battle::active_field.print_field();
}

void Battle::attack(Players player, int move, FIELD_POSITION target)
{

    int eff_atk, eff_def;
    if(move.get_damage_type() == "physical")
    {
        eff_atk = Battle::active_field.active_pokes[player].get_stat(STAT::ATK);
        eff_def = Battle::active_field.active_pokes[!player].get_stat(STAT::DEF);
    }
    else
    {
        eff_atk = Battle::active_field.active_pokes[player].get_stat(STAT::SPA);
        eff_def = Battle::active_field.active_pokes[!player].get_stat(STAT::SPD);
    }



}

bool Battle::roll_acc(float acc)
{
    return true;
}

bool Battle::roll_crit(float crit_chance)
{
    float crit = rand()/(float)RAND_MAX;
    return crit < crit_chance;
}

float calculater_damage_modifier(Move move, Field field, Pokemon attacker, int num_targets)
{
    float damage_modifier = 1;

    if(num_targets > 1)
        damage_modifier *= 0.75;

    if((field.weather_state == Weather::RAIN && move.get_type() == PokeTypes::WATER) || (field.weather_state == Weather::HARSH_SUNLIGHT && move.get_type() == PokeTypes::FIRE))
        damage_modifier *= 1.5;

    if((field.weather_state == Weather::RAIN && move.get_type() == PokeTypes::FIRE) || (field.weather_state == Weather::HARSH_SUNLIGHT && move.get_type() == PokeTypes::WATER))
        damage_modifier *= 0.5;

    if((move.get_type() == attacker.get_type()[0]) || (move.get_type() == attacker.get_type()[1]))
        damage_modifier *= 1.5;

    damage_modifier *= calculate_type_damage_modifier(attacker.get_type(), move.get_type());



    return damage_modifier;
}