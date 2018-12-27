//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_TYPE_H
#define POKEMON_SIM_TYPE_H

#include <string>

enum PokeTypes
{
    NORMAL = 0,
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
    NO_TYPE,
    NUM_TYPES
};

float calculate_damage_modifier_single(PokeTypes pokemon_type, PokeTypes move_type);

float calculate_type_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type);

bool is_stab(PokeTypes* pokemon_type, PokeTypes move_type);

PokeTypes string_to_type(std::string type);

#endif //POKEMON_SIM_TYPE_H