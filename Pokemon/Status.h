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
    NUM_NON_VOLATILE_STATUSES
};

//TODO: TRAP, TERRAIN, WEATHER, ACC, EVASION, SOUND MOVES?, INFATUATION, ENCORE, TORMENT, DISABLE, CURSED BODY
enum VOLATILE_STATUS_NUMBERS
{
    NUM_BOUND       = 0,
    NUM_CANT_ESCAPE = 1,
    NUM_CONFUSION   = 2,
    NUM_CURSE       = 3,
    NUM_EMBARGO     = 4,
    NUM_ENCORE      = 5,
    NUM_FLINCHED    = 6,
    NUM_HEALBLOCK   = 7,
    NUM_IDENTIFIED  = 8,
    NUM_INFATUATION = 9,
    NUM_NIGHTMARE   = 10,
    NUM_PERISHSONG  = 11,
    NUM_TAUNTED     = 12,
    NUM_TELEKINESIS = 13,
    NUM_TORMENT     = 14,
    NUM_VOLATILE_STATUS
};

enum VOLATILE_STATUS
{
    NONE        = 0,
    BOUND       = (1u << NUM_BOUND),
    CANT_ESCAPE = (1u << NUM_CANT_ESCAPE),
    CONFUSION   = (1u << NUM_CONFUSION),
    CURSE       = (1u << NUM_CURSE),
    EMBARGO     = (1u << NUM_EMBARGO),
    ENCORE      = (1u << NUM_ENCORE),
    FLINCHED    = (1u << NUM_FLINCHED),
    HEALBLOCK   = (1u << NUM_HEALBLOCK),
    IDENTIFIED  = (1u << NUM_IDENTIFIED),
    INFATUATION = (1u << NUM_INFATUATION),
    NIGHTMARE   = (1u << NUM_NIGHTMARE),
    PERISHSONG  = (1u << NUM_PERISHSONG),
    TAUNTED     = (1u << NUM_TAUNTED),
    TELEKINESIS = (1u << NUM_TELEKINESIS),
    TORMENT     = (1u << NUM_TORMENT),
};

static unsigned int pre_attack_v_status_mask = (VOLATILE_STATUS::CONFUSION +
                                                VOLATILE_STATUS::ENCORE +
                                                VOLATILE_STATUS::FLINCHED +
                                                VOLATILE_STATUS::TAUNTED);
static unsigned int turn_end_v_status_mask = 0;

static unsigned int turn_end_v_status_mask_clear = (VOLATILE_STATUS::FLINCHED);

STATUS string_to_status(std::string status_string);
std::string status_to_string(STATUS status);

VOLATILE_STATUS string_to_v_status(std::string status_string);
std::string v_status_to_string(VOLATILE_STATUS v_status);

#endif //POKEMON_SIM_STATUS_H
