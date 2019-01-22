//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_NATURES_H
#define POKEMON_SIM_NATURES_H

#include <Pokemon/Stats.h>

#include <string>

enum Natures
{
    HARDY = 0,
    LONELY,
    BRAVE,
    ADAMANT,
    NAUGHTY,
    BOLD,
    DOCILE,
    RELAXED,
    IMPISH,
    LAX,
    TIMID,
    HASTY,
    SERIOUS,
    JOLLY,
    NAIVE,
    MODEST,
    MILD,
    QUIET,
    BASHFUL,
    RASH,
    CALM,
    GENTLE,
    SASSY,
    CAREFUL,
    QUIRKY,
    NUM_NATURES
};

float get_nature_mod(Natures nature, STAT stat);

Natures string_to_nature(std::string nature_string);

#endif //POKEMON_SIM_NATURES_H
