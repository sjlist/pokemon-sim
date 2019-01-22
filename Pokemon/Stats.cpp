//
// Created by slist on 12/20/18.
//

#include <Pokemon/Stats.h>

#include <map>
#include <string>

static std::map<STAT, std::string> string_stat_map = {
        { STAT::HP,  "HP" },
        { STAT::ATK, "ATK" },
        { STAT::DEF, "DEF" },
        { STAT::SPA, "SPA" },
        { STAT::SPD, "SPD" },
        { STAT::SPE, "SPE" },
        { STAT::NUM_STATS, "NUM STATS" }
};

static std::map<std::string, STAT> stat_string_map = {
        { "HP",  STAT::HP  },
        { "ATK", STAT::ATK },
        { "DEF", STAT::DEF },
        { "SPA", STAT::SPA },
        { "SPD", STAT::SPD },
        { "SPE", STAT::SPE },
        { "NUM STATS", STAT::NUM_STATS }
};

std::string stat_to_string(STAT stat)
{
    return string_stat_map[stat];
}

STAT string_to_stat(std::string stat_string)
{
    return stat_string_map[stat_string];
}