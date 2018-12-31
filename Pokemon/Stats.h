//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_STATS_H
#define POKEMON_SIM_STATS_H

#include <string>

enum STAT
{
    HP = 0,
    ATK,
    DEF,
    SPA,
    SPD,
    SPE,
    NUM_STATS
};

std::string stat_to_string(STAT stat);
STAT string_to_stat(std::string stat_string);

#endif //POKEMON_SIM_STATS_H
