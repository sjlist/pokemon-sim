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
    Battle get_battle();
private:
    BattleActor actor;
    bool battle_over();
    int num_players = 2;
    int turn_count;
    Battle battle;
    void init();
    std::vector<FIELD_POSITION> create_priority_list(BattleMessage* messages);
    std::vector<FIELD_POSITION> remove_priority_list(FIELD_POSITION pos, int current_action, std::vector<FIELD_POSITION> prio_list);
};


#endif //POKEMON_SIM_BATTLESTATEMACHINE_H
