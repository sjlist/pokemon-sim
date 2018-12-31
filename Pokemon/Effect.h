//
// Created by slist on 12/27/18.
//

#ifndef POKEMON_SIM_EFFECT_H
#define POKEMON_SIM_EFFECT_H

#include <string>
#include "Status.h"
#include "Stats.h"

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
    STAT_CHANGE,
    NUM_MOVE_EFFECTS
};

class Effect {
public:
    Effect();
    void load_effect(boost::property_tree::ptree effect_tree);

    MOVE_EFFECTS get_effect();
    float get_effect_chance();

    STATUS get_effect_status_type();

    STAT get_stat_changed();
    int get_stages_changed();
private:
    MOVE_EFFECTS effect_type;
    float effect_chance;

    //Effect specific attributes
    STATUS status_effect;

    STAT stat_changed;
    int stages_changes;
};

MOVE_EFFECTS string_to_move_effect(std::string move_effect_string);

#endif //POKEMON_SIM_EFFECT_H
