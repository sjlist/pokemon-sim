//
// Created by slist on 12/27/18.
//

#ifndef POKEMON_SIM_EFFECT_H
#define POKEMON_SIM_EFFECT_H

#include <string>

enum MOVE_EFFECTS
{
    NO_MOVE_EFFECT = 0,
    FLINCH,
    SWAP,
    NUM_MOVE_EFFECTS
};

MOVE_EFFECTS string_to_move_effect(std::string move_effect_string);

#endif //POKEMON_SIM_EFFECT_H
