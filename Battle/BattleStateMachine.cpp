//
// Created by slist on 12/19/18.
//

#include "BattleStateMachine.h"
#include "BattleActor.h"
#include "Players.h"
#include "Pokemon/Move.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <algorithm>

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
    //BattleStateMachine::battle.print_battle(true);
}

void BattleStateMachine::run()
{
    BattleState state = BattleState::BATTLE_INIT;
    Attack_Result atk_r;
    Players faint_player;

    int sendout_choice [2] = {0, 0};

    BattleMessage messages [BattleStateMachine::num_players];
    messages[FIELD_POSITION::PLAYER_1_0].move_command = Commands::COMMAND_ATTACK;
    messages[FIELD_POSITION::PLAYER_1_0].move_num = 0;
    messages[FIELD_POSITION::PLAYER_1_0].target_pos = FIELD_POSITION::PLAYER_2_0;

    messages[FIELD_POSITION::PLAYER_2_0].move_command = Commands::COMMAND_ATTACK;
    messages[FIELD_POSITION::PLAYER_2_0].move_num = 0;
    messages[FIELD_POSITION::PLAYER_2_0].target_pos = FIELD_POSITION::PLAYER_1_0;

    std::vector<FIELD_POSITION> prio (BattleStateMachine::num_players);

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
                BattleStateMachine::battle.print_battle(true);
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                //get action choice

                //determine attack order
                prio = BattleStateMachine::update_priority_list(messages);

                state = BattleState::TURN_EXECUTE;
                break;

            case BattleState::TURN_EXECUTE:
                for(int i = 0; i < BattleStateMachine::num_players; i++)
                {
                    if(prio.at(i) != FIELD_POSITION::NO_POSITION)
                        atk_r = BattleStateMachine::battle.attack(prio.at(i), messages[prio.at(i)].move_num);

                    if(atk_r == Attack_Result::FAINT)
                    {
                        //determine the fainted side
                        if(i == PLAYER_ONE)
                        {
                            faint_player = PLAYER_TWO;
                        }
                        else
                        {
                            faint_player = PLAYER_ONE;
                        }

                        // lose if no pokes left
                        if(battle.has_lost(faint_player))
                        {
                            std::cout << "Player " << (faint_player + 1) << " has lost the battle!\n";
                            state = BattleState::BATTLE_END;
                            break;;
                        }

                        // remove next action in turn if there is one left. use no_player
                        for(int j = (i + 1); j < BattleStateMachine::num_players; j++)
                        {
                            if(messages[prio.at(i)].target_pos == prio.at(j))
                            {
                                prio.at(j) = FIELD_POSITION::NO_POSITION;
                            }
                        }

                    }
                }
                if(state == BattleState::TURN_EXECUTE)
                    state = BattleState::TURN_END;
                break;;
            case BattleState::TURN_END:
                state = BattleState::BATTLE_END;
        }
    }
}

std::vector<FIELD_POSITION> BattleStateMachine::update_priority_list(BattleMessage* messages)
{
    std::vector<FIELD_POSITION> prio_list (BattleStateMachine::num_players);
    int move_prio;
    std::map<int, int> prio_map;

    for(int pos = FIELD_POSITION::PLAYER_1_0; pos < FIELD_POSITION ::NUM_POSITIONS; pos++)
    {
        switch (messages[pos].move_command)
        {
            case Commands::COMMAND_SWAP:
                move_prio = MAX_PRIO + 1;
                break;
            case Commands::COMMAND_ATTACK:
                move_prio = BattleStateMachine::battle.active_field.active_pokes[pos].moves[messages[pos].move_num].get_priority();
                break;
            default:
                std::cout << "Unsupported Command " << messages[pos].move_command << "\n";
                assert(0);
        }
        prio_map[pos] = move_prio;
    }

    if(prio_map[Players::PLAYER_ONE] > prio_map[Players::PLAYER_TWO])
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_2_0;
    }
    else if(prio_map[Players::PLAYER_ONE] > prio_map[Players::PLAYER_TWO])
    {
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_ONE].get_stat(STAT::SPE) > BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_TWO].get_stat(STAT::SPE))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_2_0;
    }
    else
    {
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
    }

    return prio_list;
}