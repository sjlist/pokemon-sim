//
// Created by slist on 1/3/19.
//

#ifndef POKEMON_SIM_FIELDPOSITIONS_H
#define POKEMON_SIM_FIELDPOSITIONS_H

#include <Config.h>
using namespace std;

enum FIELD_POSITION
{
    PLAYER_1_0 = 0,
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    PLAYER_1_1,
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    PLAYER_1_2,
#endif
    PLAYER_2_0,
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    PLAYER_2_1,
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    PLAYER_2_2,
#endif
    NUM_POSITIONS,
    NO_POSITION,
    ALL_TARGETS
};

string get_string_from_field_position(FIELD_POSITION pos);

#endif //POKEMON_SIM_FIELDPOSITIONS_H
