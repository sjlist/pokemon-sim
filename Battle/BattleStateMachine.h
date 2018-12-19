//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLESTATEMACHINE_H
#define POKEMON_SIM_BATTLESTATEMACHINE_H

#include "Battle.h"

class BattleStateMachine {
public:
    void run();
private:
    Battle battle;
    void init();
};


#endif //POKEMON_SIM_BATTLESTATEMACHINE_H
