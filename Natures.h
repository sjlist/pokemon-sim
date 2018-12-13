//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_NATURES_H
#define POKEMON_SIM_NATURES_H

#include "Stats.h"

enum Natures
{
    HARDY = 0,
    LONLEY,
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

#endif //POKEMON_SIM_NATURES_H
