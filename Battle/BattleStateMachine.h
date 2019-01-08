//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLESTATEMACHINE_H
#define POKEMON_SIM_BATTLESTATEMACHINE_H

#include "Battle.h"
#include "BattleActor.h"

enum class BattleState
{
    BATTLE_INIT = 0,
    BATTLE_START,
    TURN_START,
    TURN_EXECUTE,
    TURN_END,
    BATTLE_END,
    NUM_BATTLE_STATES
};

class BattleStateMachine {
public:
    BattleStateMachine();
    BattleStateMachine(long seed);

    void run(BattleState state=BattleState::BATTLE_INIT);
    Battle get_battle();
    void reset();
private:
    void init();

    long seed;
    std::mt19937 choice;
    BattleActor actor;
    int num_players = 2;
    int turn_count;
    Battle battle;

    bool battle_over();
    std::vector<FIELD_POSITION> create_priority_list(BattleMessage* messages);
    std::vector<FIELD_POSITION> remove_priority_list(int action, std::vector<FIELD_POSITION> prio_list);
    int moves_later(FIELD_POSITION pos, int current_action, std::vector<FIELD_POSITION> prio_list);
};


#endif //POKEMON_SIM_BATTLESTATEMACHINE_H
