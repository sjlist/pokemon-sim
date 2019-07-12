//
// Created by slist on 12/20/18.
//

#include <Pokemon/Stats.h>

#include <map>
#include <string>
using namespace std;

static map<STAT, string> string_stat_map = {
        { STAT::HP,  "HP" },
        { STAT::ATK, "ATK" },
        { STAT::DEF, "DEF" },
        { STAT::SPA, "SPA" },
        { STAT::SPD, "SPD" },
        { STAT::SPE, "SPE" },
        { STAT::NUM_STATS, "NUM STATS" }
};

static map<string, STAT> stat_string_map = {
        { "HP",  STAT::HP  },
        { "ATK", STAT::ATK },
        { "DEF", STAT::DEF },
        { "SPA", STAT::SPA },
        { "SPD", STAT::SPD },
        { "SPE", STAT::SPE },
        { "NUM STATS", STAT::NUM_STATS }
};

string stat_to_string(STAT stat)
{
    return string_stat_map[stat];
}

STAT string_to_stat(string stat_string)
{
    return stat_string_map[stat_string];
}