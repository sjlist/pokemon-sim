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
    BattleStateMachine::turn_count = 0;

    BattleMessage messages [BattleStateMachine::num_players];
    messages[FIELD_POSITION::PLAYER_1_0].move_command = Commands::COMMAND_ATTACK;
    messages[FIELD_POSITION::PLAYER_1_0].move_num = 2;
    messages[FIELD_POSITION::PLAYER_1_0].target_pos = FIELD_POSITION::PLAYER_2_0;

    messages[FIELD_POSITION::PLAYER_2_0].move_command = Commands::COMMAND_ATTACK;
    messages[FIELD_POSITION::PLAYER_2_0].move_num = 2;
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

                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_0, sendout_choice[Players::PLAYER_ONE]);
                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_0, sendout_choice[Players::PLAYER_TWO]);
                std::cout << "\n\n\n--------------BATTLE START--------------\n";
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                BattleStateMachine::turn_count++;
                std::cout << "\n-------Turn " << BattleStateMachine::turn_count << " start-------\n";
                BattleStateMachine::battle.print_battle();
                //get action choice

                if(BattleStateMachine::turn_count == 2)
                {
                    messages[FIELD_POSITION::PLAYER_1_0].move_num = 0;
                    messages[FIELD_POSITION::PLAYER_2_0].move_num = 0;
                }

                //determine attack order
                prio = BattleStateMachine::create_priority_list(messages);

                state = BattleState::TURN_EXECUTE;
                break;

            case BattleState::TURN_EXECUTE:
                for(int i = 0; i < BattleStateMachine::num_players; i++)
                {
                    //Determine what attack to do
                    //if there is no attack to execute for that field position, aka it fainted earlier in the turn
                    if(prio.at(i) == FIELD_POSITION::NO_POSITION)
                        atk_r = Attack_Result::NO_ATTACK;
                    //if there needs to be a swap
                    else if(messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                        atk_r = Attack_Result::SWAP;
                    //TODO: IMPLEMENT NON DAMAGING ATTACKS
                    //default to a attack
                    else
                        atk_r = BattleStateMachine::battle.attack(prio.at(i), messages[prio.at(i)].target_pos, messages[prio.at(i)].move_num);


                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(!BattleStateMachine::battle.can_swap(get_player_from_position(prio.at(i))))
                            {
                                std::cout << "No valid pokemon to swap into for player " << (get_player_from_position(prio.at(i)) + 1) << "\n";
                                break;
                            }
                            // FOR ATTACKING MOVES THAT SWAP
                            if(messages[prio.at(i)].move_command != Commands::COMMAND_SWAP)
                            {
                                assert(0);
                                //GET SWAP STUFF FROM BATTLE ACTOR AND PUT IT IN THE CURRENT MESSAGE
                                //KEEP MESSAGE COMMAND ATTACK
                            }
                            // TODO: HANDLE PURSUIT HERE
                            BattleStateMachine::battle.swap_poke(messages[prio.at(i)].active_pos, messages[prio.at(i)].reserve_poke);
                            if(messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                                break;;

                            if(BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos].is_alive())
                                break;;

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            faint_player = get_player_from_position(messages[prio.at(i)].target_pos);

                            // lose if no pokes left
                            if(battle.has_lost(faint_player))
                            {
                                std::cout << "Player " << (faint_player + 1) << " has lost the battle!\n";
                                state = BattleState::BATTLE_END;
                                break;;
                            }

                            // remove next action in turn if there is one left. use no_player
                            prio = BattleStateMachine::remove_priority_list(messages[prio.at(i)].target_pos, i, prio);

                            if(BattleStateMachine::battle_over())
                                state = BattleState::BATTLE_END;
                            else
                                //SWAP IN A NEW POKEMON by querying the battle actor
                                BattleStateMachine::battle.swap_poke(messages[prio.at(i)].target_pos,
                                                                     BattleStateMachine::actor.choose_pokemon(
                                                                             BattleStateMachine::battle.get_party(
                                                                                     get_player_from_position(prio.at(i)))));

                            break;;

                        case Attack_Result::FLINCHED:
                            prio = BattleStateMachine::remove_priority_list(messages[prio.at(i)].target_pos, i, prio);
                            break;;

                        case Attack_Result::HIT:
                        case Attack_Result::IMMUNE:
                        case Attack_Result::NO_PP:
                        case Attack_Result::MISS:
                        case Attack_Result::NO_ATTACK:
                            break;;
                        default:
                            std::cout << "Unhandled attack result\n";
                            assert(0);
                    }

                    if(state == BattleState::BATTLE_END)
                        break;
                }

                if(state == BattleState::TURN_EXECUTE)
                    state = BattleState::TURN_END;
                break;;
            case BattleState::TURN_END:
                if(battle.handle_end_turn_field_status())
                {
                    if(battle.has_lost(Players::PLAYER_ONE))
                    {
                        std::cout << "Player One has lost the battle\n";
                        state = BattleState::BATTLE_END;
                    }
                    if(battle.has_lost(Players::PLAYER_TWO))
                    {
                        std::cout << "Player Two has lost the battle\n";
                        state = BattleState::BATTLE_END;
                    }
                }
                if(state == BattleState::TURN_END)
                    state = BattleState::TURN_START;
                break;;
            default:
                assert(0);
        }
    }
}

std::vector<FIELD_POSITION> BattleStateMachine::create_priority_list(BattleMessage* messages)
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

std::vector<FIELD_POSITION> BattleStateMachine::remove_priority_list(FIELD_POSITION pos, int current_action, std::vector<FIELD_POSITION> prio_list)
{
    for(int j = (current_action + 1); j < BattleStateMachine::num_players; j++)
    {
        if(pos == prio_list.at(j))
        {
            prio_list.at(j) = FIELD_POSITION::NO_POSITION;
            break;
        }
    }
    return prio_list;
}


Battle BattleStateMachine::get_battle()
{
    return BattleStateMachine::battle;
}

bool BattleStateMachine::battle_over()
{
    return battle.has_lost(Players::PLAYER_ONE) || battle.has_lost(Players::PLAYER_ONE);
}