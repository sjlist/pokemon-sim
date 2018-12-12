//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_TYPE_H
#define POKEMON_SIM_TYPE_H

enum class PokeTypes
{
    NORMAL = 0,
    FIRE,
    FIGHTING,
    WATER,
    FLYING,
    GRASS,
    POISON,
    ELECTRIC,
    GROUND,
    PSYCHIC,
    ROCK,
    ICE,
    BUG,
    DRAGON,
    GHOST,
    DARK,
    STEEL,
    FAIRY,
    NONE
};

float calculate_damage_modifier(PokeTypes* pokemon_type, PokeTypes move_type);

#endif //POKEMON_SIM_TYPE_H
