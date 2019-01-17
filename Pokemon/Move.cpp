//
// Created by slist on 12/12/18.
//

#include "Move.h"
#include "Status.h"
#include "fileIO/loadJSON.h"
#include "Type.h"
#include "Battle/Targeting.h"
#include "Utils/Logging.h"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <algorithm>
#include <map>

#include <iostream>

static std::map<std::string, move_damage_type> string_move_damage_type_map = {
        { "NONE", move_damage_type::NO_DAMAGE_TYPE},
        { "physical", move_damage_type::MOVE_PHYSICAL },
        { "special", move_damage_type::MOVE_SPECIAL},
        { "status", move_damage_type::MOVE_STATUS}
};

Move::Move()
{}

//GET FUNCS
std::string Move::get_name()
{
    return Move::name;
}

int Move::get_pp()
{
    return Move::current_pp;
}

int Move::get_power()
{
    return Move::power;
}

float Move::get_acc()
{
    return Move::acc;
}

float Move::get_crit()
{
    return Move::crit_chance;
}

PokeTypes Move::get_type()
{
    return Move::type;
}

move_damage_type Move::get_damage_type()
{
    return Move::damage_type;
}

int Move::get_priority()
{
    return Move::priority;
}

Effect Move::get_move_effect(int effect_num)
{
    if(effect_num == -1)
        return Move::move_effects.back();
    else
        return Move::move_effects[effect_num];
}

TARGETS Move::get_move_targets()
{
    return Move::move_targets;
}

int Move::get_num_targets()
{
    return Move::num_targets;
}

// SET FUNCS
bool Move::use()
{
    if(Move::current_pp > 0)
    {
        Move::current_pp--;
        return true;
    }
    else
    {
        return false;
    }
}

// Loading Moves
void Move::reset()
{
    Move::current_pp = Move::max_pp;
}

void Move::load_move(std::string move_name)
{
    Move::name = move_name;
    boost::property_tree::ptree move_tree, tmp_tree;

    try
    {
        move_tree = load_json_file("Moves/" + move_name + ".json");
        DEBUG_MSG("read move " << move_name << "\n");
    }
    catch(...)
    {
        assert(0);
    }

    Move::type = string_to_type(move_tree.get<std::string>("type"));

    Move::acc = (float)move_tree.get<int>("acc", 0) / 100;
    Move::max_pp = move_tree.get<int>("pp", 0);
    Move::current_pp = Move::max_pp;
    Move::priority = move_tree.get<int>("priority", 0);
    Move::damage_type = string_move_damage_type_map[move_tree.get<std::string>("damage_type")];

    Move::move_targets = string_to_target(move_tree.get<std::string>("targeting"));
    Move::num_targets = move_tree.get<int>("num_targets");

    if(move_tree.count("damage_info"))
    {
        tmp_tree = move_tree.get_child("damage_info");
        Move::power = tmp_tree.get<int>("power");
        Move::crit_chance = (float) tmp_tree.get<int>("crit_chance") / 100;
    } else
    {
        Move::power = 0;
        Move::crit_chance = 0;
    }

    if(move_tree.count("effects"))
    {
        tmp_tree = move_tree.get_child("effects");
        int i = 0;
        while(tmp_tree.count("effect" + std::to_string(i)))
        {
            Move::move_effects[i].load_effect(tmp_tree.get_child("effect" + std::to_string(i)));
            i++;
        }
    }
}


//Printing moves
void Move::print_move()
{
#ifdef DEBUG
    DEBUG_MSG("Name: " << Move::name << "\n");
    DEBUG_MSG("Damage Type: " << Move::damage_type << "\n");
    DEBUG_MSG("Type: " << Move::type << "\n");
    DEBUG_MSG("Power: " << Move::power << "\n");
    DEBUG_MSG("Acc: " << Move::acc << "\n");
    DEBUG_MSG("PP: " << Move::current_pp << "/" << Move::max_pp << "\n");
    DEBUG_MSG("Priority: " << Move::priority << "\n");
    DEBUG_MSG("Crit chance: " << Move::crit_chance << "\n");
#endif
}