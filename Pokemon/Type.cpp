//
// Created by slist on 12/12/18.
//

#include <Pokemon/Type.h>

#include <map>
using namespace std;

static float type_effectiveness [(int)PokeTypes::NUM_TYPES] [(int)PokeTypes::NUM_TYPES] = { // MOVE TYPE, POKE TYPE
       //NONE,   NORMAL, FIRE,   WATER, GRASS, ELECTRIC, ICE,   FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG,   ROCK,  GHOST, DRAGON, DARK,  STEEL, FAIRY
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   ST_DM, ST_DM, ST_DM, ST_DM,  ST_DM, ST_DM, ST_DM}, // NONE
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   ST_DM, NE_DM, IM_DM, ST_DM,  ST_DM, NE_DM, ST_DM}, // NORMAL
        {ST_DM,  ST_DM,  NE_DM,  NE_DM, SE_DM, ST_DM,    SE_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   SE_DM, NE_DM, ST_DM, NE_DM,  ST_DM, SE_DM, ST_DM}, // FIRE
        {ST_DM,  ST_DM,  SE_DM,  NE_DM, NE_DM, ST_DM,    ST_DM, ST_DM,    ST_DM,  SE_DM,  ST_DM,  ST_DM,   ST_DM, SE_DM, ST_DM, NE_DM,  ST_DM, ST_DM, ST_DM}, // WATER
        {ST_DM,  ST_DM,  NE_DM,  SE_DM, NE_DM, ST_DM,    ST_DM, ST_DM,    NE_DM,  SE_DM,  NE_DM,  ST_DM,   NE_DM, SE_DM, ST_DM, NE_DM,  ST_DM, NE_DM, ST_DM}, // GRASS
        {ST_DM,  ST_DM,  ST_DM,  SE_DM, NE_DM, NE_DM,    ST_DM, ST_DM,    ST_DM,  IM_DM,  SE_DM,  ST_DM,   ST_DM, ST_DM, ST_DM, NE_DM,  ST_DM, ST_DM, ST_DM}, // ELECTRIC
        {ST_DM,  ST_DM,  NE_DM,  NE_DM, SE_DM, ST_DM,    NE_DM, ST_DM,    ST_DM,  SE_DM,  SE_DM,  ST_DM,   ST_DM, ST_DM, ST_DM, SE_DM,  ST_DM, NE_DM, ST_DM}, // ICE
        {ST_DM,  SE_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    SE_DM, ST_DM,    NE_DM,  ST_DM,  NE_DM,  NE_DM,   NE_DM, SE_DM, IM_DM, ST_DM,  SE_DM, SE_DM, NE_DM}, // FIGHTING
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, SE_DM, ST_DM,    ST_DM, ST_DM,    NE_DM,  NE_DM,  ST_DM,  ST_DM,   ST_DM, NE_DM, NE_DM, ST_DM,  ST_DM, IM_DM, SE_DM}, // POISON
        {ST_DM,  ST_DM,  SE_DM,  ST_DM, NE_DM, SE_DM,    ST_DM, ST_DM,    SE_DM,  ST_DM,  IM_DM,  ST_DM,   NE_DM, SE_DM, ST_DM, ST_DM,  ST_DM, SE_DM, ST_DM}, // GROUND
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, SE_DM, NE_DM,    ST_DM, SE_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   SE_DM, NE_DM, ST_DM, ST_DM,  ST_DM, NE_DM, ST_DM}, // FLYING
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, SE_DM,    SE_DM,  ST_DM,  ST_DM,  NE_DM,   ST_DM, ST_DM, ST_DM, ST_DM,  IM_DM, NE_DM, ST_DM}, // PSYCHIC
        {ST_DM,  ST_DM,  NE_DM,  ST_DM, SE_DM, ST_DM,    ST_DM, NE_DM,    NE_DM,  ST_DM,  NE_DM,  SE_DM,   ST_DM, ST_DM, NE_DM, ST_DM,  SE_DM, NE_DM, NE_DM}, // BUG
        {ST_DM,  ST_DM,  SE_DM,  ST_DM, ST_DM, ST_DM,    SE_DM, NE_DM,    ST_DM,  NE_DM,  SE_DM,  ST_DM,   SE_DM, ST_DM, ST_DM, ST_DM,  ST_DM, NE_DM, ST_DM}, // ROCK
        {ST_DM,  IM_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  SE_DM,   ST_DM, ST_DM, SE_DM, ST_DM,  NE_DM, ST_DM, ST_DM}, // GHOST
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   ST_DM, ST_DM, ST_DM, SE_DM,  ST_DM, NE_DM, IM_DM}, // DRAGON
        {ST_DM,  ST_DM,  ST_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, NE_DM,    ST_DM,  ST_DM,  ST_DM,  SE_DM,   ST_DM, ST_DM, SE_DM, ST_DM,  NE_DM, ST_DM, NE_DM}, // DARK
        {ST_DM,  ST_DM,  NE_DM,  NE_DM, ST_DM, ST_DM,    SE_DM, ST_DM,    ST_DM,  ST_DM,  ST_DM,  ST_DM,   ST_DM, SE_DM, ST_DM, ST_DM,  ST_DM, NE_DM, SE_DM}, // STEEL
        {ST_DM,  ST_DM,  NE_DM,  ST_DM, ST_DM, ST_DM,    ST_DM, SE_DM,    NE_DM,  ST_DM,  ST_DM,  ST_DM,   ST_DM, ST_DM, ST_DM, SE_DM,  SE_DM, NE_DM, ST_DM}  // FAIRY
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