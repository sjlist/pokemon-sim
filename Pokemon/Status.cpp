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

STATUS string_to_status(std::string status_string)
{
    return string_status_map[status_string];
}

std::string status_to_string(STATUS status)
{
    return status_string_map[status];
}