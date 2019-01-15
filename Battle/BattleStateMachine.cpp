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
#include <random>

#define DEBUGGING

BattleStateMachine::BattleStateMachine()
{
    std::random_device rd;
    BattleStateMachine::seed = rd();
    std::cout << "The random seed is " << BattleStateMachine::seed << "\n";
    BattleStateMachine::choice = std::mt19937(seed);
    BattleStateMachine::actor = BattleActor(seed);
    BattleStateMachine::battle = Battle(seed);
}

BattleStateMachine::BattleStateMachine(long seed)
{
    BattleStateMachine::seed = seed;
    std::cout << "The random seed is " << BattleStateMachine::seed << "\n";
    BattleStateMachine::choice = std::mt19937(seed);
    BattleStateMachine::actor = BattleActor(seed);
    BattleStateMachine::battle = Battle(seed);
}

void BattleStateMachine::init()
{
    BattleStateMachine::battle.load_battle();
    //BattleStateMachine::battle.print_battle(true);
}

int BattleStateMachine::run(BattleState state)
{
    Attack_Result atk_r;
    Players faint_player;
    int MAX_TURN_COUNT = 200;
    int winner = 0, removed, count;

    BattleStateMachine::turn_count = 0;

    BattleMessage messages [BattleStateMachine::num_players];
    std::vector<FIELD_POSITION> prio (BattleStateMachine::num_players);

    while(true)
    {
        switch (state)
        {
            case BattleState::BATTLE_INIT:
                BattleStateMachine::init();
                state = BattleState::BATTLE_START;
                break;;
            case BattleState::BATTLE_START:
                //get pokemon to send out

                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    BattleStateMachine::battle.send_out(
                            static_cast<FIELD_POSITION>(i),
                            actor.choose_pokemon(
                                    BattleStateMachine::battle.get_party(
                                            get_player_from_position(i)
                                            )));
                }
                std::cout << "\n\n\n--------------BATTLE START--------------\n";
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                BattleStateMachine::turn_count++;
#ifdef DEBUGGING
                if(BattleStateMachine::turn_count == 17)

                    std::cout << "HERERE\n";
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                    if(BattleStateMachine::battle.active_field.active_pokes[i].get_current_hp() == 0)
                        assert(0);
                if(BattleStateMachine::turn_count >= MAX_TURN_COUNT)
                    assert(0);
#endif
                std::cout << "\n-------Turn " << BattleStateMachine::turn_count << " start-------\n";
                BattleStateMachine::battle.print_battle();
                //get action choice
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    messages[i] = BattleStateMachine::actor.choose_action(
                            static_cast<FIELD_POSITION>(i),
                            BattleStateMachine::battle.get_party(get_player_from_position(static_cast<FIELD_POSITION>(i))),
                            BattleStateMachine::battle.active_field);
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
                    //default to a attack
                    else if(messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK)
                        atk_r = BattleStateMachine::battle.attack(prio.at(i), messages[prio.at(i)].target_pos, messages[prio.at(i)].move_num);
                    else
                        assert(0);

                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(!BattleStateMachine::battle.can_swap(get_player_from_position(prio.at(i))))
                            {
                                std::cout << "No valid pokemon to swap into for player " << (get_player_from_position(prio.at(i)) + 1) << "\n";
                            }
                            else
                            {
                                // FOR ATTACKING MOVES THAT SWAP
                                if (messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK) {
                                    FIELD_POSITION target = messages[prio.at(i)].target_pos;
                                    messages[prio.at(i)] = BattleStateMachine::actor.choose_action(
                                            prio.at(i),
                                            BattleStateMachine::battle.get_party(get_player_from_position(prio.at(i))),
                                            BattleStateMachine::battle.active_field,
                                            Actions::CHOOSE_POKEMON);
                                    messages[prio.at(i)].move_command = Commands::COMMAND_ATTACK;
                                    messages[prio.at(i)].target_pos = target;
                                    //GET SWAP STUFF FROM BATTLE ACTOR AND PUT IT IN THE CURRENT MESSAGE
                                    //KEEP MESSAGE COMMAND ATTACK
                                }

                                // TODO: HANDLE PURSUIT HERE
                                while(!BattleStateMachine::battle.swap_poke(messages[prio.at(i)].active_pos,
                                                                     messages[prio.at(i)].reserve_poke))
                                {
                                    if(BattleStateMachine::battle_over())
                                        return BattleStateMachine::end_battle();

                                    FIELD_POSITION target = messages[prio.at(i)].target_pos;
                                    messages[prio.at(i)] = BattleStateMachine::actor.choose_action(
                                            prio.at(i),
                                            BattleStateMachine::battle.get_party(get_player_from_position(prio.at(i))),
                                            BattleStateMachine::battle.active_field,
                                            Actions::CHOOSE_POKEMON);
                                    messages[prio.at(i)].move_command = Commands::COMMAND_ATTACK;
                                    messages[prio.at(i)].target_pos = target;
                                }

                                if (messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                                    break;;

                                if (BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos].is_alive())
                                    break;;
                            }

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            for(int p = 0; p < FIELD_POSITION::NUM_POSITIONS; p++)
                            {
                                if(!BattleStateMachine::battle.active_field.active_pokes[p].is_alive())
                                    faint_player = get_player_from_position(static_cast<FIELD_POSITION>(p));
                                else
                                    continue;

                                // lose if no pokes left
                                if (battle.has_lost(faint_player)) {
                                    state = BattleState::BATTLE_END;
                                    continue;
                                }

                                // remove next action in turn if there is one left. use no_player
                                removed = BattleStateMachine::moves_later(static_cast<FIELD_POSITION>(p), i, prio);
                                if (removed != -1)
                                    prio = BattleStateMachine::remove_priority_list(removed, prio);

                                if (BattleStateMachine::battle_over())
                                    state = BattleState::BATTLE_END;
                                else
                                    //SWAP IN A NEW POKEMON by querying the battle actor
                                    while(!BattleStateMachine::battle.swap_poke(static_cast<FIELD_POSITION>(p),
                                                                         BattleStateMachine::actor.choose_pokemon(
                                                                                 BattleStateMachine::battle.get_party(
                                                                                         get_player_from_position(p
                                                                                         )))))
                                    {
                                        if(BattleStateMachine::battle.has_lost(get_player_from_position(static_cast<FIELD_POSITION>(p))))
                                            break;

                                    }

                                contin:;
                            }
                            if(BattleStateMachine::battle_over())
                                return BattleStateMachine::end_battle();
                            break;;

                        case Attack_Result::FLINCHED:
                            removed = BattleStateMachine::moves_later(messages[prio.at(i)].target_pos, i, prio);
                            if( removed != -1 )
                            {
                                std::cout << "P" << get_player_from_position(messages[prio.at(i)].target_pos) + 1 << "'s "
                                          << BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos].get_species() << " flinched\n";

                                prio = BattleStateMachine::remove_priority_list(removed, prio);
                            }
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

                    if(state != BattleState::TURN_EXECUTE)
                        break;
                }

                if(state == BattleState::TURN_EXECUTE)
                    state = BattleState::TURN_END;
                break;;
            case BattleState::TURN_END:
                if(battle.handle_end_turn_field_status())
                {
                    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                    {
                        if(!BattleStateMachine::battle.active_field.active_pokes[static_cast<FIELD_POSITION>(i)].is_alive())
                        {
                            if(battle.has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
                            {
                                state = BattleState::BATTLE_END;
                            }
                            else
                            {
                                while(!BattleStateMachine::battle.swap_poke(static_cast<FIELD_POSITION>(i),
                                                                     BattleStateMachine::actor.choose_pokemon(
                                                                             BattleStateMachine::battle.get_party(
                                                                                     get_player_from_position(static_cast<FIELD_POSITION>(i)
                                                                                     )))))
                                {
                                    if(BattleStateMachine::battle.has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
                                    {
                                        state = BattleState::BATTLE_END;
                                        break;
                                    }
                                }



                            }
                        }
                    }
                }
                if(BattleStateMachine::battle_over())
                    state = BattleState::BATTLE_END;

                if(state == BattleState::TURN_END)
                    state = BattleState::TURN_START;
                break;;
            case BattleState::BATTLE_END:
                return BattleStateMachine::end_battle();
            default:
                assert(0);
        }
    }

}

int BattleStateMachine::end_battle()
{

    int loser = 0;
    if(BattleStateMachine::battle.has_lost(Players::PLAYER_ONE) && BattleStateMachine::battle.has_lost(Players::PLAYER_TWO))
        loser = 0;
    else if(BattleStateMachine::battle.has_lost(Players::PLAYER_ONE))
        loser -= 1;
    else if(BattleStateMachine::battle.has_lost(Players::PLAYER_TWO))
        loser += 1;
    else
        assert(0);

    if(loser != 0)
    {
        std::cout << "Player " << ((loser + 1) / 2) + 1 << " has lost the battle!\n";
    }
    else
    {
        std::cout << "TIED\n";
    }


    return loser;
}

std::vector<FIELD_POSITION> BattleStateMachine::create_priority_list(BattleMessage* messages)
{
    std::vector<FIELD_POSITION> prio_list (BattleStateMachine::num_players);
    int move_prio;
    std::map<int, int> prio_map;

    for(int pos = FIELD_POSITION::PLAYER_1_0; pos < FIELD_POSITION ::NUM_POSITIONS; pos++)
    {
        switch(messages[pos].move_command)
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
    else if(prio_map[Players::PLAYER_ONE] < prio_map[Players::PLAYER_TWO])
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_ONE].get_stat(STAT::SPE) > BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_TWO].get_stat(STAT::SPE))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_2_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_ONE].get_stat(STAT::SPE) < BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_TWO].get_stat(STAT::SPE))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_ONE].get_stat(STAT::SPE) == BattleStateMachine::battle.active_field.active_pokes[Players::PLAYER_TWO].get_stat(STAT::SPE))
    {
        int choice = BattleStateMachine::make_choice(0, 1);
        prio_list.at(0) = static_cast<FIELD_POSITION>(choice);
        prio_list.at(1) = static_cast<FIELD_POSITION>(!choice);
    }
    else
        assert(0);
    return prio_list;
}

int BattleStateMachine::moves_later(FIELD_POSITION pos, int current_action, std::vector<FIELD_POSITION> prio_list)
{
    for(int j = (current_action + 1); j < BattleStateMachine::num_players; j++)
    {
        if (pos == prio_list.at(j))
        {
            return j;
        }
    }
    return -1;
}

std::vector<FIELD_POSITION> BattleStateMachine::remove_priority_list(int action, std::vector<FIELD_POSITION> prio_list)
{
    prio_list.at(action) = FIELD_POSITION::NO_POSITION;
    return prio_list;
}


Battle BattleStateMachine::get_battle()
{
    return BattleStateMachine::battle;
}

int BattleStateMachine::get_turn_count()
{
    return BattleStateMachine::turn_count;
}

long BattleStateMachine::get_seed()
{
    return BattleStateMachine::seed;
}

bool BattleStateMachine::battle_over()
{
    return battle.has_lost(Players::PLAYER_ONE) || battle.has_lost(Players::PLAYER_TWO);
}

void BattleStateMachine::reset()
{
    std::cout << "------RESETTING BATTLE------\n";
    std::random_device rd;
    BattleStateMachine::seed = rd();
    std::cout << "New random seed: " << seed << std::endl;
    BattleStateMachine::turn_count = 0;
    BattleStateMachine::battle.reset();
    BattleStateMachine::battle.update_generator(seed);
    BattleStateMachine::actor.update_generator(seed);
}

int BattleStateMachine::make_choice(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(BattleStateMachine::choice);
}
