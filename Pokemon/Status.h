//
// Created by slist on 12/18/18.
//

#ifndef POKEMON_SIM_STATUS_H
#define POKEMON_SIM_STATUS_H

#include <string>

enum STATUS
{
    NO_STATUS = 0,
    BURNED,
    FROZEN,
    PARALYZED,
    POISONED,
    BADLY_POISONED,
    ASLEEP,
    NUM_STATUSES
};

STATUS string_to_status(std::string status_string);
std::string status_to_string(STATUS status);

#endif //POKEMON_SIM_STATUS_H
