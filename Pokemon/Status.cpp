//
// Created by slist on 12/18/18.
//

#include "Status.h"
#include "Pokemon/Pokemon.h"

#include <string>
#include <map>

std::map<std::string, STATUS> string_status_map = {
        { "PARALYZED", STATUS::PARALYZED },
        { "POISONED", STATUS::POISONED },
        { "BADLY_POISONED", STATUS::BADLY_POISONED },
        { "FROZEN", STATUS::FROZEN },
        { "ASLEEP", STATUS::ASLEEP },
        { "BURNED", STATUS::BURNED },
        { "NONE", STATUS::NO_STATUS }
};

std::map<STATUS, std::string> status_string_map = {
        { STATUS::PARALYZED, "PARALYZED"},
        { STATUS::POISONED , "POISONED"},
        { STATUS::BADLY_POISONED, "BADLY_POISONED" },
        { STATUS::FROZEN, "FROZEN" },
        { STATUS::ASLEEP, "ASLEEP" },
        { STATUS::BURNED, "BURNED" },
        { STATUS::NO_STATUS, "NONE" }
};

std::map<std::string, VOLITILE_STATUS> string_v_status_map = {
        { "BOUND", VOLITILE_STATUS::BOUND },
        { "CANT_ESCAPE", VOLITILE_STATUS::CANT_ESCAPE },
        { "CONFUSION", VOLITILE_STATUS::CONFUSION },
        { "CURSE", VOLITILE_STATUS::CURSE },
        { "EMBARGO", VOLITILE_STATUS::EMBARGO },
        { "ENCORE", VOLITILE_STATUS::ENCORE },
        { "FLINCHED", VOLITILE_STATUS::FLINCHED },
        { "HEALBLOCK", VOLITILE_STATUS::HEALBLOCK },
        { "IDENTIFIED", VOLITILE_STATUS::IDENTIFIED },
        { "INFATUATION", VOLITILE_STATUS::INFATUATION },
        { "NIGHTMARE", VOLITILE_STATUS::NIGHTMARE },
        { "PERISHSONG", VOLITILE_STATUS::PERISHSONG },
        { "TAUNT", VOLITILE_STATUS::TAUNT },
        { "TELEKINESIS", VOLITILE_STATUS::TELEKINESIS },
        { "TORMENT", VOLITILE_STATUS::TORMENT }
};

std::map<VOLITILE_STATUS, std::string> v_status_string_map = {
        { VOLITILE_STATUS::BOUND, "BOUND" },
        { VOLITILE_STATUS::CANT_ESCAPE, "CANT ESCAPE" },
        { VOLITILE_STATUS::CONFUSION, "CONFUSED" },
        { VOLITILE_STATUS::CURSE, "CURSED" },
        { VOLITILE_STATUS::EMBARGO, "EMBARGO'D" },
        { VOLITILE_STATUS::ENCORE, "ENCORED" },
        { VOLITILE_STATUS::FLINCHED, "FLINCHED" },
        { VOLITILE_STATUS::HEALBLOCK, "HEALBLOCKED" },
        { VOLITILE_STATUS::IDENTIFIED, "IDENTIFIED" },
        { VOLITILE_STATUS::INFATUATION, "INFATUATED" },
        { VOLITILE_STATUS::NIGHTMARE, "NIGHTMARE'D" },
        { VOLITILE_STATUS::PERISHSONG, "PERISHSONG'D" },
        { VOLITILE_STATUS::TAUNT, "TAUNTED" },
        { VOLITILE_STATUS::TELEKINESIS, "TELEKINESIS'D" },
        { VOLITILE_STATUS::TORMENT, "TORMENTED" }
};

STATUS string_to_status(std::string status_string)
{
    return string_status_map[status_string];
}

std::string status_to_string(STATUS status)
{
    return status_string_map[status];
}

VOLITILE_STATUS string_to_v_status(std::string status_string)
{
    return string_v_status_map[status_string];
}

std::string v_status_to_string(VOLITILE_STATUS v_status)
{
    return v_status_string_map[v_status];
}