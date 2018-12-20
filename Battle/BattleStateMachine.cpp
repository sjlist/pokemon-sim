//
// Created by slist on 12/19/18.
//

#include "BattleStateMachine.h"
#include "BattleActor.h"
#include "Players.h"
#include <cstdlib>

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


void BattleStateMachine::init()
{
    BattleStateMachine::battle.load_battle();
    std::srand(time(NULL));
    BattleStateMachine::battle.print_battle(true);
}

void BattleStateMachine::run()
{
    BattleState state = BattleState::BATTLE_INIT;
    int sendout_choice [2] = {0, 0};

    while(state != BattleState::BATTLE_END)
    {
        switch (state)
        {
            case BattleState::BATTLE_INIT:
                BattleStateMachine::init();
                state = BattleState::BATTLE_START;
                break;;
            case BattleState::BATTLE_START:
                //get pokemon to send out

                BattleStateMachine::battle.send_out(Players::PLAYER_ONE, sendout_choice[Players::PLAYER_ONE]);
                BattleStateMachine::battle.send_out(Players::PLAYER_TWO, sendout_choice[Players::PLAYER_TWO]);
                BattleStateMachine::battle.print_battle();
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                state = BattleState::BATTLE_END;

        }
    }
}