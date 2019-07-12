//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_STATS_H
#define POKEMON_SIM_STATS_H

#include <string>
using namespace std;

enum STAT
{
    HP = 0,
    ATK,
    DEF,
    SPA,
    SPD,
    SPE,
    ACC,
    EVA,
    NUM_STATS
};

string stat_to_string(STAT stat);
STAT string_to_stat(string stat_string);

#endif //POKEMON_SIM_STATS_H
