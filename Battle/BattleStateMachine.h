//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLESTATEMACHINE_H
#define POKEMON_SIM_BATTLESTATEMACHINE_H

#include "Battle.h"
#include "BattleActor.h"

class BattleStateMachine {
public:
    void run();
private:
    int num_players = 2;
    Battle battle;
    void init();
    std::vector<FIELD_POSITION> update_priority_list(BattleMessage* messages);
};


#endif //POKEMON_SIM_BATTLESTATEMACHINE_H
