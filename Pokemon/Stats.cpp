//
// Created by slist on 12/20/18.
//

#include "Stats.h"
#include <string>
#include <map>

static std::map<STAT, std::string> string_stat_map = {
        { STAT::HP,  "HP" },
        { STAT::ATK, "ATK" },
        { STAT::DEF, "DEF" },
        { STAT::SPA, "SPA" },
        { STAT::SPD, "SPD" },
        { STAT::SPE, "SPE" },
        { STAT::NUM_STATS, "NUM STATS" }
};


std::string stat_to_string(STAT stat)
{
    return string_stat_map[stat];
}