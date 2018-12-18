//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_MOVE_H
#define POKEMON_SIM_MOVE_H

#include <string>
#include "Type.h"
#include "Status.h"

class Move {
public:
    Move();

    int get_pp();
    int get_power();
    int get_acc();
    PokeTypes get_type();
    std::string get_name();
    float get_status_chance();
    STATUS get_status_effect();
    void load_move(std::string move_name);
    void print_move();

private:
    std::string name;
    int max_pp;
    int current_pp;
    int power;
    float acc;
    int priority;
    STATUS status_effect;
    float status_chance;
    PokeTypes type;
};


#endif //POKEMON_SIM_MOVE_H
