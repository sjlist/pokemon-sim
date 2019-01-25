//
// Created by slist on 12/27/18.
//

#ifndef POKEMON_SIM_EFFECT_H
#define POKEMON_SIM_EFFECT_H

#include <Battle/Field_Objects.h>
#include <Battle/Field_Positions.h>
#include <Pokemon/Stats.h>
#include <Pokemon/Status.h>
#include <Pokemon/Type.h>

#include <boost/property_tree/ptree.hpp>
#include <string>

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
    HEAL,
    REMOVE_TYPE,
    FLAT_DAMAGE,
    PROTECT,
    SUBSTITUTE,
    NUM_MOVE_EFFECTS
};

class Effect {
public:
    Effect();
    void load_effect(boost::property_tree::ptree effect_tree);

    MOVE_EFFECTS get_effect();
    float get_effect_chance();
    bool does_target_self();

    VOLATILE_STATUS get_volatile_status_effect();

    STATUS get_effect_status_type();

    Field_Objects get_field_obj_changed();

    STAT get_stat_changed();
    int get_stages_changed();

    bool get_use_damage();
    float get_percent_recoil();

    float get_heal_percent();

    PokeTypes get_type_removed();

    bool use_flat_level();
    int get_flat_damage();
private:
    MOVE_EFFECTS effect_type;
    float effect_chance;
    bool target_self;

    //Effect specific attributes
    STATUS status_effect;

    VOLATILE_STATUS v_status_effect;

    Field_Objects field_obj;

    STAT stat_changed;
    int stages_changes;

    bool use_damage;
    float percent_recoil;

    float heal_percent;

    PokeTypes type_removed;

    bool flat_damage_level;
    int flat_damage;
};

MOVE_EFFECTS string_to_move_effect(std::string move_effect_string);

#endif //POKEMON_SIM_EFFECT_H
