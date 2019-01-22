//
// Created by slist on 12/18/18.
//

#include <Pokemon/Pokemon.h>
#include <Pokemon/Status.h>

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

std::map<std::string, VOLATILE_STATUS> string_v_status_map = {
        { "BOUND", VOLATILE_STATUS::BOUND },
        { "CANT_ESCAPE", VOLATILE_STATUS::CANT_ESCAPE },
        { "CONFUSION", VOLATILE_STATUS::CONFUSION },
        { "CURSE", VOLATILE_STATUS::CURSE },
        { "EMBARGO", VOLATILE_STATUS::EMBARGO },
        { "ENCORE", VOLATILE_STATUS::ENCORE },
        { "FLINCHED", VOLATILE_STATUS::FLINCHED },
        { "HEALBLOCK", VOLATILE_STATUS::HEALBLOCK },
        { "IDENTIFIED", VOLATILE_STATUS::IDENTIFIED },
        { "INFATUATION", VOLATILE_STATUS::INFATUATION },
        { "NIGHTMARE", VOLATILE_STATUS::NIGHTMARE },
        { "PERISHSONG", VOLATILE_STATUS::PERISHSONG },
        { "TAUNT", VOLATILE_STATUS::TAUNTED },
        { "TELEKINESIS", VOLATILE_STATUS::TELEKINESIS },
        { "TORMENT", VOLATILE_STATUS::TORMENT }
};

std::map<VOLATILE_STATUS, std::string> v_status_string_map = {
        { VOLATILE_STATUS::BOUND, "BOUND" },
        { VOLATILE_STATUS::CANT_ESCAPE, "CANT ESCAPE" },
        { VOLATILE_STATUS::CONFUSION, "CONFUSED" },
        { VOLATILE_STATUS::CURSE, "CURSED" },
        { VOLATILE_STATUS::EMBARGO, "EMBARGO'D" },
        { VOLATILE_STATUS::ENCORE, "ENCORED" },
        { VOLATILE_STATUS::FLINCHED, "FLINCHED" },
        { VOLATILE_STATUS::HEALBLOCK, "HEALBLOCKED" },
        { VOLATILE_STATUS::IDENTIFIED, "IDENTIFIED" },
        { VOLATILE_STATUS::INFATUATION, "INFATUATED" },
        { VOLATILE_STATUS::NIGHTMARE, "NIGHTMARE'D" },
        { VOLATILE_STATUS::PERISHSONG, "PERISHSONG'D" },
        { VOLATILE_STATUS::TAUNTED, "TAUNTED" },
        { VOLATILE_STATUS::TELEKINESIS, "TELEKINESIS'D" },
        { VOLATILE_STATUS::TORMENT, "TORMENTED" }
};

STATUS string_to_status(std::string status_string)
{
    return string_status_map[status_string];
}

std::string status_to_string(STATUS status)
{
    return status_string_map[status];
}

VOLATILE_STATUS string_to_v_status(std::string status_string)
{
    return string_v_status_map[status_string];
}

std::string v_status_to_string(VOLATILE_STATUS v_status)
{
    return v_status_string_map[v_status];
}