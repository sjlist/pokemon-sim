//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_BATTLE_H
#define POKEMON_SIM_BATTLE_H

#include "Pokemon.h"
#include "Field.h"

class Battle {
public:
    Battle();
    Field active_field;
private:
    Pokemon p1_party [6];
    Pokemon p2_party [6];
};


#endif //POKEMON_SIM_BATTLE_H
