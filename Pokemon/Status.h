//
// Created by slist on 12/18/18.
//

#ifndef POKEMON_SIM_STATUS_H
#define POKEMON_SIM_STATUS_H

#include <string>

enum STATUS
{
    BURNED = 0,
    FROZEN,
    PARALYZED,
    POISONED,
    ASLEEP,
    NO_STATUS,
    NUM_STATUSES
};

STATUS string_to_status(std::string status_string);

#endif //POKEMON_SIM_STATUS_H
