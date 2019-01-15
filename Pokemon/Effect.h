//
// Created by slist on 12/27/18.
//

#ifndef POKEMON_SIM_EFFECT_H
#define POKEMON_SIM_EFFECT_H

#include <string>
#include "Status.h"
#include "Stats.h"

#include <boost/property_tree/ptree.hpp>
#include <Battle/Field_Objects.h>

/*
 * Effects are the general non damaging effects that a move can have, stat bufff, status application ...
 */
enum MOVE_EFFECTS
{
    NO_MOVE_EFFECT = 0,
    NON_VOLATILE_STATUS_EFFECT,
    VOLATILE_STATUS_EFFECT,
    FLINCH,
    SWAP,
    STAT_CHANGE,
    FIELD_CHANGE,
    RECOIL,
    NUM_MOVE_EFFECTS
};

class Effect {
public:
    Effect();
    void load_effect(boost::property_tree::ptree effect_tree);

    MOVE_EFFECTS get_effect();
    float get_effect_chance();

    VOLATILE_STATUS get_volatile_status_effect();

    STATUS get_effect_status_type();

    Field_Objects get_field_obj_changed();

    STAT get_stat_changed();
    int get_stages_changed();

    bool get_use_damage();
    float get_percent_recoil();
private:
    MOVE_EFFECTS effect_type;
    float effect_chance;

    //Effect specific attributes
    STATUS status_effect;

    VOLATILE_STATUS v_status_effect;

    Field_Objects field_obj;

    STAT stat_changed;
    int stages_changes;

    bool use_damage;
    float percent_recoil;
};

MOVE_EFFECTS string_to_move_effect(std::string move_effect_string);

#endif //POKEMON_SIM_EFFECT_H
