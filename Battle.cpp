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

// Battle init occurs after teams are loaded in
void Battle::init()
{

}
