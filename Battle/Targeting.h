//
// Created by slist on 1/3/19.
//

#ifndef POKEMON_SIM_TARGETING_H
#define POKEMON_SIM_TARGETING_H

#include <Battle/Field_Positions.h>

#include <string>

//Nameing Scheme of targeting
//Relative position to attacking pokemon: adjacent, all, others, or self (special case)
//Teams that are relevent to pick from: friendly, enemy, all
//Number of targets: all or one
enum TARGETS
{
    //ADJACTENT
    ADJACENT_ALL = 0,
    ADJACENT_ENEMY,
    ADJACENT_FRIENDLY,

    //ALL
    ALL_ALL,
    ALL_OTHERS,
    ALL_ENEMY,
    ALL_FRIENDLY,

    //SELF
    SELF
};

class Targets {
public:
    Targets();
    FIELD_POSITION valid_targets [FIELD_POSITION::NUM_POSITIONS];

    int get_num_valid_targets();
    FIELD_POSITION get_single_target();

    void set_single_target(FIELD_POSITION pos);

    void get_valid_targets(TARGETS attack_target, FIELD_POSITION atk_pos);
private:
    int num_valid_targets;
    FIELD_POSITION single_target;
};

TARGETS string_to_target(std::string target_string);

#endif //POKEMON_SIM_TARGETING_H
