//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_MOVE_H
#define POKEMON_SIM_MOVE_H

#include <string>
#include "Type.h"
#include "Status.h"
#include "Effect.h"

static const int MAX_PRIO = 5;
static const int MAX_EFFECTS = 4;

enum move_damage_type
{
    NO_DAMAGE_TYPE = 0, //BAD, put here for poor move intiailization
    MOVE_PHYSICAL,
    MOVE_SPECIAL,
    MOVE_STATUS,
    NUM_MOVE_DAMAGE_TYPES
};

class Move {
public:
    Move();

    bool use();
    int get_pp();
    int get_power();
    float get_acc();
    Effect get_move_effect(int effect_num);
    PokeTypes get_type();
    std::string get_name();
    float get_status_chance();
    STATUS get_status_effect();
    int get_priority();

    void load_move(std::string move_name);
    void print_move();
    move_damage_type get_damage_type();

private:
    std::string name;
    int max_pp;
    int power;
    float acc;
    int priority;
    Effect move_effects[MAX_EFFECTS];
    float crit_chance;
    PokeTypes type;
    move_damage_type damage_type;

    int current_pp;
};


#endif //POKEMON_SIM_MOVE_H
