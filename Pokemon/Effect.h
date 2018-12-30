//
// Created by slist on 12/27/18.
//

#ifndef POKEMON_SIM_EFFECT_H
#define POKEMON_SIM_EFFECT_H

#include <string>
#include "Status.h"

#include <boost/property_tree/ptree.hpp>

/*
 * Effects are the general non damaging effects that a move can have, stat bufff, status application ...
 */

enum MOVE_EFFECTS
{
    NO_MOVE_EFFECT = 0,
    STATUS_EFFECT,
    FLINCH,
    SWAP,
    NUM_MOVE_EFFECTS
};

class Effect {
public:
    Effect();
    void load_effect(boost::property_tree::ptree effect_tree);

    MOVE_EFFECTS get_effect();
    STATUS get_effect_status_type();
    float get_effect_chance();

private:
    MOVE_EFFECTS effect_type;
    STATUS status_effect;
    float effect_chance;
};

MOVE_EFFECTS string_to_move_effect(std::string move_effect_string);

#endif //POKEMON_SIM_EFFECT_H
