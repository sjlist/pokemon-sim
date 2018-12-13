//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_MOVE_H
#define POKEMON_SIM_MOVE_H

#include <string>
#include "Type.h"

class Move {
public:
    Move();

    int get_pp();
    int get_power();
    int get_acc();
    PokeTypes get_type();
    std::string get_name();


private:
    std::string name;
    int max_pp;
    int current_pp;
    int power;
    int acc;
    PokeTypes type;
};


#endif //POKEMON_SIM_MOVE_H
