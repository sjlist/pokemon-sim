//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_MOVE_H
#define POKEMON_SIM_MOVE_H

#include <string>
#include "Type.h"

class Move {
public:
    std::string name;
    int max_pp;
    int current_pp;
    int power;
    PokeTypes type[2];

};


#endif //POKEMON_SIM_MOVE_H
