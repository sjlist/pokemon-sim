//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_TYPE_H
#define POKEMON_SIM_TYPE_H

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

float calculate_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type);

#endif //POKEMON_SIM_TYPE_H
