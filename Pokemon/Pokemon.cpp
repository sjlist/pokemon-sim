//
// Created by slist on 12/12/18.
//

#include <string>
#include <iostream>
#include <cassert>
#include <cmath>

#include "Pokemon.h"
#include "Stats.h"
#include "Move.h"
#include "Natures.h"
#include "fileIO/loadJSON.h"
#include "Type.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>

static int MAX_STAGES = 6;

Pokemon::Pokemon()
{
    Pokemon::active = false;
    Pokemon::status = STATUS::NO_STATUS;
    for(int stat = STAT::HP; stat < STAT::NUM_STATS; stat++)
    {
        Pokemon::stat_modifiers[stat] = 0;
    }
    Pokemon::alive = false;
}

// STATE CHECKING FUNCTIONS
bool Pokemon::is_active()
{
    return Pokemon::active;
}

int Pokemon::get_stat(STAT stat) //TODO: Implement dynamic stat adjustment
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

std::string Pokemon::get_species()
{
    return Pokemon::species;
}

bool Pokemon::is_alive()
{
    return Pokemon::alive;
}

// STATE CHANGE FUNCTIONS

bool Pokemon::use_move(int move_number)
{
    return Pokemon::moves[move_number].use();
}

bool Pokemon::deal_damage(int damage)
{
    Pokemon::current_hp = Pokemon::current_hp - damage;

    if(Pokemon::current_hp <= 0)
    {
        Pokemon::current_hp = 0;
        return false;
    }
    return true;
}

void Pokemon::set_status(STATUS new_status)
{
    Pokemon::status = new_status;
}

void Pokemon::set_active(bool state)
{
    Pokemon::active = state;
}

void Pokemon::faint_poke()
{
    Pokemon::alive = false;
}

void Pokemon::stat_change(STAT stat, int stages)
{
    if(stages < 0)
        std::cout << Pokemon::species << "'s " << stat_to_string(stat) << " dropped by " << std::to_string(abs(stages)) << " stage(s)\n";

    if(stages > 0)
        std::cout << Pokemon::species << "'s " << stat_to_string(stat) << " rose by" << std::to_string(stages) << " stage(s)\n";

    Pokemon::stat_modifiers[stat] = Pokemon::stat_modifiers[stat] + stages;

    if(Pokemon::stat_modifiers[stat] < (-1 * MAX_STAGES))
    {
        std::cout << Pokemon::species << "'s " << stat_to_string(stat) << " can't go lower\n";
        Pokemon::stat_modifiers[stat] = -1 * MAX_STAGES;
    }

    if(Pokemon::stat_modifiers[stat] > MAX_STAGES)
    {
        std::cout << Pokemon::species << "'s " << stat_to_string(stat) << " can't go higher\n";
        Pokemon::stat_modifiers[stat] = MAX_STAGES;
    }
}
//

// LOADING POKEMON FUNCTIONS

void Pokemon::load_pokemon(boost::property_tree::ptree poke_ptree)
{

    int* b = Pokemon::load_species(poke_ptree.get<std::string>("species"));
    int base [6];

    for(int i = 0; i < STAT::NUM_STATS; i++)
        base[i] = *(b + i);


    int evs [6], ivs [6];
    auto ev_ptr = evs;
    auto iv_ptr = ivs;

    Pokemon::level = poke_ptree.get<int>("level", 0);

    boost::property_tree::ptree evs_ptree = poke_ptree.get_child("evs");
    boost::property_tree::ptree ivs_ptree = poke_ptree.get_child("ivs");

    for(int i = 0; i < STAT::NUM_STATS; i++)
    {
        evs[i] = evs_ptree.get(std::to_string(i), 0);
        ivs[i] = ivs_ptree.get(std::to_string(i), 0);
    }

    Pokemon::set_stats(base, iv_ptr, ev_ptr, level, string_to_nature(poke_ptree.get<std::string>("nature")));

    boost::property_tree::ptree moves_ptree = poke_ptree.get_child("moves");


    std::string move_name;
    for(int i = 0; i < 4; i++)
    {
        move_name = moves_ptree.get<std::string>("MOVE" + std::to_string(i));
        if(move_name != "")
        {
            Pokemon::moves[i].load_move(move_name);
        }
    }

    Pokemon::alive = true;
}

int* Pokemon::load_species(std::string species_name)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree root_child;
    int base_stats [6];
    auto base_ptr = base_stats;

    try
    {
        root = load_json_file("species/" + species_name + ".json");
    }
    catch(...)
    {
        std::cout << "ERROR: FAILED TO LOAD SPECIES " << species_name << "\n";
        assert(0);
    }

    base_stats[STAT::HP]  = root.get<int>("HP", 0);
    base_stats[STAT::ATK] = root.get<int>("ATK", 0);
    base_stats[STAT::DEF] = root.get<int>("DEF", 0);
    base_stats[STAT::SPA] = root.get<int>("SPA", 0);
    base_stats[STAT::SPD] = root.get<int>("SPD", 0);
    base_stats[STAT::SPE] = root.get<int>("SPE", 0);

    Pokemon::species = species_name;

    root_child = root.get_child("TYPES");
    Pokemon::type[0] = string_to_type(root_child.get<std::string>("TYPE0"));
    Pokemon::type[1] = string_to_type(root_child.get<std::string>("TYPE1"));
    return base_ptr;
}

int Pokemon::calculate_hp(int level, int base_hp, int ev_hp, int iv_hp)
{
    return std::floor(((2 * base_hp + iv_hp + std::floor((float)(ev_hp / 4))) * level) / 100) + level + 10;
}

int Pokemon::calculate_stat_single(int level, int base, int ev, int iv, float nature_mod)
{
    return (std::floor((float)std::floor((float)(ev / 4) + iv + 2 * base) / 100 * level) + 5) * nature_mod;
}

void Pokemon::set_stats(int* base, int* ivs, int* evs, int level, Natures nature)
{
    float nature_mod;
    for(int i = STAT::HP; i < STAT::NUM_STATS; i++)
    {
        if(i != STAT::HP)
        {
            nature_mod = get_nature_mod(nature, (STAT)i);
            Pokemon::base_stats[i] = calculate_stat_single(level, base[i], evs[i], ivs[i], nature_mod);
        }
        else
        {
            //std::cout << evs[i] << " " << i << "\n";

            Pokemon::base_stats[i] = calculate_hp(level, base[i], evs[i], ivs[i]);
            Pokemon::current_hp = Pokemon::base_stats[i];
        }
    }
}
//


void Pokemon::print_pokemon(bool detailed)
{
    std::cout << Pokemon::species << "\n";
    if(detailed)
    {
        std::cout << "HP:  "  << Pokemon::base_stats[STAT::HP] << " Modifier: " << Pokemon::stat_modifiers[STAT::HP]  << "\n";
        std::cout << "ATK: " << Pokemon::base_stats[STAT::ATK] << " Modifier: " << Pokemon::stat_modifiers[STAT::ATK] << "\n";
        std::cout << "DEF: " << Pokemon::base_stats[STAT::DEF] << " Modifier: " << Pokemon::stat_modifiers[STAT::DEF] << "\n";
        std::cout << "SPA: " << Pokemon::base_stats[STAT::SPA] << " Modifier: " << Pokemon::stat_modifiers[STAT::SPA] << "\n";
        std::cout << "SPD: " << Pokemon::base_stats[STAT::SPD] << " Modifier: " << Pokemon::stat_modifiers[STAT::SPD] << "\n";
        std::cout << "SPE: " << Pokemon::base_stats[STAT::SPE] << " Modifier: " << Pokemon::stat_modifiers[STAT::SPE] << "\n";
        std::cout << "Level: " << Pokemon::level << "\n";
    }
    std::cout << "Current HP: " << Pokemon::current_hp << "\n";
    std::cout << "ACTIVE: " << Pokemon::active << "\n";
    std::cout << "ALIVE: " << Pokemon::alive << "\n";
}
