//
// Created by slist on 12/12/18.
//

#include <string>
#include <iostream>
#include <cassert>

#include "Pokemon.h"
#include "Move.h"
#include "Natures.h"
#include "loadJSON.h"
#include <boost/property_tree/ptree.hpp>

Pokemon::Pokemon()
{
    Pokemon::active = false;
}

void Pokemon::load_pokemon(boost::property_tree::ptree poke_ptree)
{
    Pokemon::load_species(poke_ptree.get<std::string>("species"));
    Pokemon::level = poke_ptree.get<int>("level", 0);
}

void Pokemon::load_species(std::string species_name)
{
    std::cout << species_name << "\n";
    boost::property_tree::ptree root;

    try
    {
        root = load_json_file("species/" + species_name + ".json");
    }
    catch(...)
    {
        std::cout << "ERROR: FAILED TO LOAD SPECIES " << species_name << "\n";
        assert(0);
    }

    Pokemon::base_stats[STAT::HP] = root.get<int>("HP", 0);
    Pokemon::base_stats[STAT::ATK] = root.get<int>("ATK", 0);
    Pokemon::base_stats[STAT::DEF] = root.get<int>("DEF", 0);
    Pokemon::base_stats[STAT::SPA] = root.get<int>("SPA", 0);
    Pokemon::base_stats[STAT::SPD] = root.get<int>("SPD", 0);
    Pokemon::base_stats[STAT::SPE] = root.get<int>("SPE", 0);
    Pokemon::species = species_name;
    std::cout << Pokemon::base_stats[0] << "\n";
}

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

bool Pokemon::use_move(int move_number)
{
    return Pokemon::moves[move_number].use();
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

void Pokemon::print_pokemon()
{

}
