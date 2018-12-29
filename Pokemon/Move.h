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
    MOVE_EFFECTS get_move_effect();
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
    STATUS status_effect;
    MOVE_EFFECTS effect;
    float status_chance;
    float crit_chance;
    PokeTypes type;
    move_damage_type damage_type;

    int current_pp;
};


#endif //POKEMON_SIM_MOVE_H
