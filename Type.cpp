//
// Created by slist on 12/12/18.
//

#include "Type.h"
#include <map>
#include <iostream>


static float type_effectiveness [(int)PokeTypes::NUM_TYPES] [(int)PokeTypes::NUM_TYPES] = {
       //NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY, NONE
        {1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 0.5,   0.0,  1.0,    1.0,   0.5,  1.0,    1.0}, // NORMAL
        {1.0,    0.5,  0.5,   2.0,   1.0,      2.0, 1.0,      1.0,    1.0,     1.0,   1.0,     2.0, 0.5,   1.0,  0.5,    1.0,   2.0,  1.0,    1.0}, // FIRE
        {1.0,    2.0,  0.5,   0.5,   1.0,      1.0, 1.0,      1.0,    2.0,     1.0,   1.0,     1.0, 2.0,   1.0,  0.5,    1.0,   1.0,  1.0,    1.0}, // WATER
        {1.0,    0.5,  2.0,   0.5,   1.0,      1.0, 1.0,      0.5,    2.0,     0.5,   1.0,     0.5, 2.0,   1.0,  0.5,    1.0,   0.5,  1.0,    1.0}, // GRASS
        {1.0,    1.0,  2.0,   0.5,   0.5,      1.0, 1.0,      1.0,    0.0,     2.0,   1.0,     1.0, 1.0,   1.0,  0.5,    1.0,   1.0,  1.0,    1.0}, // ELECTRIC
        {1.0,    0.5,  0.5,   2.0,   1.0,      0.5, 1.0,      1.0,    2.0,     2.0,   1.0,     1.0, 1.0,   1.0,  2.0,    1.0,   0.5,  1.0,    1.0}, // ICE
        {2.0,    1.0,  1.0,   1.0,   1.0,      2.0, 1.0,      0.5,    1.0,     0.5,   0.5,     0.5, 2.0,   0.0,  1.0,    2.0,   2.0,  0.5,    1.0}, // FIGHTING
        {1.0,    1.0,  1.0,   2.0,   1.0,      1.0, 1.0,      0.5,    0.5,     1.0,   1.0,     1.0, 0.5,   0.5,  1.0,    1.0,   0.0,  2.0,    1.0}, // POISON
        {1.0,    2.0,  1.0,   0.5,   2.0,      1.0, 1.0,      2.0,    1.0,     0.0,   1.0,     0.5, 2.0,   1.0,  1.0,    1.0,   2.0,  1.0,    1.0}, // GROUND
        {1.0,    1.0,  1.0,   2.0,   0.5,      1.0, 2.0,      1.0,    1.0,     1.0,   1.0,     2.0, 0.5,   1.0,  1.0,    1.0,   0.5,  1.0,    1.0}, // FLYING
        {1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 2.0,      2.0,    1.0,     1.0,   0.5,     1.0, 1.0,   1.0,  1.0,    0.0,   0.5,  1.0,    1.0}, // PSYCHIC
        {1.0,    0.5,  1.0,   2.0,   1.0,      1.0, 0.5,      0.5,    1.0,     0.5,   2.0,     1.0, 1.0,   0.5,  1.0,    2.0,   0.5,  0.5,    1.0}, // BUG
        {1.0,    2.0,  1.0,   1.0,   1.0,      2.0, 0.5,      1.0,    0.5,     2.0,   1.0,     2.0, 1.0,   1.0,  1.0,    1.0,   0.5,  1.0,    1.0}, // ROCK
        {0.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   2.0,     1.0, 1.0,   2.0,  1.0,    0.5,   0.5,  1.0,    1.0}, // GHOST
        {1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  2.0,    1.0,   0.5,  0.0,    1.0}, // DRAGON
        {1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 0.5,      1.0,    1.0,     1.0,   2.0,     1.0, 1.0,   2.0,  1.0,    0.5,   0.5,  0.5,    1.0}, // DARK
        {1.0,    0.5,  0.5,   1.0,   1.0,      2.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 2.0,   1.0,  1.0,    1.0,   0.5,  2.0,    1.0}, // STEEL
        {1.0,    0.5,  1.0,   1.0,   1.0,      1.0, 2.0,      0.5,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  2.0,    2.0,   0.5,  1.0,    1.0}, // FAIRY
        {1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  1.0,    1.0,   1.0,  1.0,    1.0}  // NONE
};

static std::map<std::string, PokeTypes> string_types = {
        {"NORMAL",   PokeTypes::NORMAL},
        {"FIRE",     PokeTypes::FIRE},
        {"WATER",    PokeTypes::WATER},
        {"GRASS",    PokeTypes::GRASS},
        {"ELECTRIC", PokeTypes::ELECTRIC},
        {"ICE",      PokeTypes::ICE},
        {"FIGHTING", PokeTypes::FIGHTING},
        {"POISON",   PokeTypes::POISON},
        {"GROUND",   PokeTypes::GROUND},
        {"FLYING",   PokeTypes::FLYING},
        {"PSYCHIC",  PokeTypes::PSYCHIC},
        {"BUG",      PokeTypes::BUG},
        {"ROCK",     PokeTypes::ROCK},
        {"GHOST",    PokeTypes::GHOST},
        {"DRAGON",   PokeTypes::DRAGON},
        {"DARK",     PokeTypes::DARK},
        {"STEEL",    PokeTypes::STEEL},
        {"FAIRY",    PokeTypes::FAIRY},
        {"NONE",     PokeTypes::NO_TYPE}
};

float calculate_damage_modifier_single(PokeTypes pokemon_type, PokeTypes move_type)
{
    return type_effectiveness[move_type][pokemon_type];
}

float calculate_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type)
{
    return calculate_damage_modifier_single(pokemon_type[0], move_type) * calculate_damage_modifier_single(pokemon_type[1], move_type);
}

PokeTypes string_to_type(std::string type)
{
    return string_types[type];
}