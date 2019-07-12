//
// Created by slist on 12/12/18.
//

#include <Pokemon/Type.h>

#include <map>
using namespace std;

static float type_effectiveness [(int)PokeTypes::NUM_TYPES] [(int)PokeTypes::NUM_TYPES] = {
       //NONE, NORMAL, FIRE, WATER, GRASS, ELECTRIC, ICE, FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON, DARK, STEEL, FAIRY
        {1.0,  1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  1.0,    1.0,   1.0,  1.0}, // NONE
        {1.0,  1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 0.5,   0.0,  1.0,    1.0,   0.5,  1.0}, // NORMAL
        {1.0,  1.0,    0.5,  0.5,   2.0,   1.0,      2.0, 1.0,      1.0,    1.0,     1.0,   1.0,     2.0, 0.5,   1.0,  0.5,    1.0,   2.0,  1.0}, // FIRE
        {1.0,  1.0,    2.0,  0.5,   0.5,   1.0,      1.0, 1.0,      1.0,    2.0,     1.0,   1.0,     1.0, 2.0,   1.0,  0.5,    1.0,   1.0,  1.0}, // WATER
        {1.0,  1.0,    0.5,  2.0,   0.5,   1.0,      1.0, 1.0,      0.5,    2.0,     0.5,   1.0,     0.5, 2.0,   1.0,  0.5,    1.0,   0.5,  1.0}, // GRASS
        {1.0,  1.0,    1.0,  2.0,   0.5,   0.5,      1.0, 1.0,      1.0,    0.0,     2.0,   1.0,     1.0, 1.0,   1.0,  0.5,    1.0,   1.0,  1.0}, // ELECTRIC
        {1.0,  1.0,    0.5,  0.5,   2.0,   1.0,      0.5, 1.0,      1.0,    2.0,     2.0,   1.0,     1.0, 1.0,   1.0,  2.0,    1.0,   0.5,  1.0}, // ICE
        {1.0,  2.0,    1.0,  1.0,   1.0,   1.0,      2.0, 1.0,      0.5,    1.0,     0.5,   0.5,     0.5, 2.0,   0.0,  1.0,    2.0,   2.0,  0.5}, // FIGHTING
        {1.0,  1.0,    1.0,  1.0,   2.0,   1.0,      1.0, 1.0,      0.5,    0.5,     1.0,   1.0,     1.0, 0.5,   0.5,  1.0,    1.0,   0.0,  2.0}, // POISON
        {1.0,  1.0,    2.0,  1.0,   0.5,   2.0,      1.0, 1.0,      2.0,    1.0,     0.0,   1.0,     0.5, 2.0,   1.0,  1.0,    1.0,   2.0,  1.0}, // GROUND
        {1.0,  1.0,    1.0,  1.0,   2.0,   0.5,      1.0, 2.0,      1.0,    1.0,     1.0,   1.0,     2.0, 0.5,   1.0,  1.0,    1.0,   0.5,  1.0}, // FLYING
        {1.0,  1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 2.0,      2.0,    1.0,     1.0,   0.5,     1.0, 1.0,   1.0,  1.0,    0.0,   0.5,  1.0}, // PSYCHIC
        {1.0,  1.0,    0.5,  1.0,   2.0,   1.0,      1.0, 0.5,      0.5,    1.0,     0.5,   2.0,     1.0, 1.0,   0.5,  1.0,    2.0,   0.5,  0.5}, // BUG
        {1.0,  1.0,    2.0,  1.0,   1.0,   1.0,      2.0, 0.5,      1.0,    0.5,     2.0,   1.0,     2.0, 1.0,   1.0,  1.0,    1.0,   0.5,  1.0}, // ROCK
        {1.0,  0.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   2.0,     1.0, 1.0,   2.0,  1.0,    0.5,   0.5,  1.0}, // GHOST
        {1.0,  1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  2.0,    1.0,   0.5,  0.0}, // DRAGON
        {1.0,  1.0,    1.0,  1.0,   1.0,   1.0,      1.0, 0.5,      1.0,    1.0,     1.0,   2.0,     1.0, 1.0,   2.0,  1.0,    0.5,   0.5,  0.5}, // DARK
        {1.0,  1.0,    0.5,  0.5,   1.0,   1.0,      2.0, 1.0,      1.0,    1.0,     1.0,   1.0,     1.0, 2.0,   1.0,  1.0,    1.0,   0.5,  2.0}, // STEEL
        {1.0,  1.0,    0.5,  1.0,   1.0,   1.0,      1.0, 2.0,      0.5,    1.0,     1.0,   1.0,     1.0, 1.0,   1.0,  2.0,    2.0,   0.5,  1.0}  // FAIRY
};

static map<string, PokeTypes> string_types = {
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

static map<PokeTypes, string> type_strings = {
        {PokeTypes::NORMAL,   "NORMAL"},
        {PokeTypes::FIRE,     "FIRE"},
        {PokeTypes::WATER,    "WATER"},
        {PokeTypes::GRASS,    "GRASS"},
        {PokeTypes::ELECTRIC, "ELECTRIC"},
        {PokeTypes::ICE,      "ICE"},
        {PokeTypes::FIGHTING, "FIGHTING"},
        {PokeTypes::POISON,   "POISON"},
        {PokeTypes::GROUND,   "GROUND"},
        {PokeTypes::FLYING,   "FLYING"},
        {PokeTypes::PSYCHIC,  "PSYCHIC"},
        {PokeTypes::BUG,      "BUG"},
        {PokeTypes::ROCK,     "ROCK"},
        {PokeTypes::GHOST,    "GHOST"},
        {PokeTypes::DRAGON,   "DRAGON"},
        {PokeTypes::DARK,     "DARK"},
        {PokeTypes::STEEL,    "STEEL"},
        {PokeTypes::FAIRY,    "FAIRY"},
        {PokeTypes::NO_TYPE,  "NONE"}
};


float calculate_damage_modifier_single(PokeTypes pokemon_type, PokeTypes move_type)
{
    return type_effectiveness[move_type][pokemon_type];
}

float calculate_type_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type)
{
    return calculate_damage_modifier_single(pokemon_type[0], move_type) * calculate_damage_modifier_single(pokemon_type[1], move_type);
}

PokeTypes string_to_type(string type)
{
    return string_types[type];
}

string type_to_string(PokeTypes type)
{
    return type_strings[type];
}

bool is_stab(PokeTypes* pokemon_type, PokeTypes move_type)
{
    return (pokemon_type[0] == move_type || (pokemon_type[1] == move_type && pokemon_type[1] != PokeTypes::NO_TYPE));
}