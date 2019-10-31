//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_TYPE_H
#define POKEMON_SIM_TYPE_H

#include <string>
using namespace std;

#define SE_DM 2
#define ST_DM 1
#define NE_DM 0.5
#define IM_DM 0

enum PokeTypes
{
    NO_TYPE = 0,
    NORMAL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    ICE,
    FIGHTING,
    POISON,
    GROUND,
    FLYING,
    PSYCHIC,
    BUG,
    ROCK,
    GHOST,
    DRAGON,
    DARK,
    STEEL,
    FAIRY,
    NUM_TYPES
};

float calculate_damage_modifier_single(PokeTypes pokemon_type, PokeTypes move_type);

float calculate_type_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type);

bool is_stab(PokeTypes* pokemon_type, PokeTypes move_type);

PokeTypes string_to_type(string type);
string type_to_string(PokeTypes type);


#endif //POKEMON_SIM_TYPE_H
