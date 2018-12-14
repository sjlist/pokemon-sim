//
// Created by slist on 12/12/18.
//

#include <string>
/*#include <json/json.h>
#include <fstream>*/
#include <iostream>

#include "Pokemon.h"
#include "Move.h"
#include "Natures.h"

Pokemon::Pokemon()
{
    Pokemon::name = "";
    Pokemon::active = false;
    Pokemon::current_hp = 100;
    Pokemon::level = 50;
    Pokemon::base_stats[0] = 100;
    Pokemon::base_stats[1] = 100;
    Pokemon::base_stats[2] = 100;
    Pokemon::base_stats[3] = 100;
    Pokemon::base_stats[4] = 100;
    Pokemon::base_stats[5] = 100;
    Pokemon::type[0] = PokeTypes::GROUND;
    Pokemon::type[1] = PokeTypes::DRAGON;

    Move move_1 = Move();
    Pokemon::moves[0] = move_1;
}

/*
bool Pokemon::load_species_data(std::string species)
{
    std::ifstream ifs("species/" + species + ".json");
    Json::Value obj;

    if (Json::CharReader::parse(ifs, obj))
    {
        std::cout << obj["HP"] << "\n";
        return true;
    }
    return false;
}
*/

bool Pokemon::is_active()
{
    return Pokemon::active;
}

int Pokemon::get_stat(STAT stat)
{
    return Pokemon::base_stats[stat];
}

int Pokemon::get_level()
{
    return Pokemon::level;
}

PokeTypes* Pokemon::get_type()
{
    return Pokemon::type;
}


int Pokemon::calculate_hp(int level, int base_hp, int ev_hp, int iv_hp)
{
    return ((2 * base_hp + iv_hp + (ev_hp / 4)) * level) / 100 + level + 10;
}

int Pokemon::calculate_stat_single(int level, int base, int ev, int iv, float nature_mod)
{
    return (((2 * base + iv + (ev /4)) * level) / 100 + 5) * nature_mod;
}

void Pokemon::set_stats(int* base, int* ivs, int* evs, int level, Natures nature)
{
    float nature_mod;
    for(int i = STAT::ATK; i < STAT::NUM_STATS; i++)
    {
        if(i != STAT::HP)
        {
            nature_mod = get_nature_mod(nature, (STAT)i);
            Pokemon::base_stats[i] = calculate_stat_single(level, base[i], evs[i], ivs[i], nature_mod);

        }
        else
        {
            Pokemon::base_stats[i] = calculate_hp(level, base[i], evs[i], ivs[i]);
            Pokemon::current_hp = Pokemon::base_stats[i];
        }
    }
}