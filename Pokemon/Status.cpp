//
// Created by slist on 12/18/18.
//

#include "Status.h"
#include <string>
#include <map>

std::map<std::string, STATUS> string_status_map = {
        { "PARALYZED", STATUS::PARALYZED },
        { "POISONED", STATUS::POISONED },
        { "FROZEN", STATUS::FROZEN },
        { "ASLEEP", STATUS::ASLEEP },
        { "BURNED", STATUS::BURNED },
        { "NONE", STATUS::NO_STATUS }
};

STATUS string_to_status(std::string status_string)
{
    return string_status_map[status_string];
}