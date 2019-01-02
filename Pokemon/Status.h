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
    NUM_NON_VOLITILE_STATUSES
};

enum VOLITILE_STATUS
{
    BOUND       = (1u << 0),
    CANT_ESCAPE = (1u << 1),
    CONFUSION   = (1u << 2),
    CURSE       = (1u << 3),
    EMBARGO     = (1u << 4),
    ENCORE      = (1u << 5),
    FLINCHED    = (1u << 6),
    HEALBLOCK   = (1u << 7),
    IDENTIFIED  = (1u << 8),
    INFATUATION = (1u << 9),
    NIGHTMARE   = (1u << 10),
    PERISHSONG  = (1u << 11),
    TAUNT       = (1u << 12),
    TELEKINESIS = (1u << 13),
    TORMENT     = (1u << 14),
    NUM_VOLITILE_STATUS
};

STATUS string_to_status(std::string status_string);
std::string status_to_string(STATUS status);

VOLITILE_STATUS string_to_v_status(std::string status_string);
std::string v_status_to_string(VOLITILE_STATUS v_status);

#endif //POKEMON_SIM_STATUS_H
