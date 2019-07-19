//
// Created by slist on 12/19/18.
//

#include <Battle/BattleStateMachine.h>
#include <Battle/Players.h>
#include <Config.h>
#include <fileIO/loadJSON.h>
#include <Pokemon/Move.h>
#include <Utils/Logging.h>
#include <Battle/BattleMessage.h>

#include <boost/property_tree/ptree.hpp>
#include <cstdlib>
#include <map>
#include <random>
using namespace std;

BattleStateMachine::BattleStateMachine()
{
    random_device rd;
    BattleStateMachine::seed = rd();
    DEBUG_MSG("The random seed is " << BattleStateMachine::seed << "\n");
    BattleStateMachine::choice = mt19937(seed);
    BattleStateMachine::position_choice = uniform_int_distribution<int> {0, 599};
    BattleStateMachine::battle = Battle(seed);
    BattleStateMachine::state = BattleState::BATTLE_IDLE;
}

void BattleStateMachine::init(Players player, string team_name)
{
    BattleStateMachine::battle.load_battle(player, team_name);
}

void BattleStateMachine::update_random_seed(long seed)
{
    BattleStateMachine::seed = seed;
    DEBUG_MSG("The random seed is " << BattleStateMachine::seed << "\n");
    BattleStateMachine::choice = mt19937(seed);
    BattleStateMachine::battle.update_generator(seed);
}
void BattleStateMachine::update_random_seed()
{
    random_device rd;
    BattleStateMachine::seed = rd();
    DEBUG_MSG("The random seed is " << BattleStateMachine::seed << "\n");
    BattleStateMachine::choice = mt19937(seed);
}

pair<BattleNotification, FIELD_POSITION> BattleStateMachine::run(BattleMessage message)
{
    validate_battle_message(message);

    Attack_Result atk_r = Attack_Result::NO_ATTACK;
    BattleMessage action_message;
    Players faint_player;
    FIELD_POSITION swap_pos;
    int reserve_poke;

    while(true)
    {
        switch (state)
        {
            case BattleState::BATTLE_IDLE:
                state = BattleState::BATTLE_INIT;
                return make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
            case BattleState::BATTLE_INIT:
                BattleStateMachine::init(get_player_from_position(message.pos), message.team_name);
                DEBUG_MSG("Loaded Player " << get_string_from_field_position(message.pos) << endl);
                if(BattleStateMachine::battle.get_party(PLAYER_ONE)->loaded && BattleStateMachine::battle.get_party(PLAYER_TWO)->loaded)
                {
                    state = BattleState::BATTLE_START;
                    //BattleStateMachine::battle.print_battle(true);
                    break;
                }
                else if(!BattleStateMachine::battle.get_party(PLAYER_ONE)->loaded)
                    return make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
                else if(!BattleStateMachine::battle.get_party(PLAYER_TWO)->loaded)
                    return make_pair(BattleNotification::TEAM_CHOICE, PLAYER_2_0);
                else
                    ERR_MSG("UNREACHABLE during team loading");

            case BattleState::BATTLE_START:
                turn_count = 0;

                //get pokemon to send out

                for(int i = 0; i < NUM_POSITIONS; i++)
                {
                    if(BattleStateMachine::battle.active_field.active_pokes[i] == nullptr)
                    {
                        if (message.pos == static_cast<FIELD_POSITION>(i))
                        {
                            BattleStateMachine::battle.send_out(static_cast<FIELD_POSITION>(i), message.reserve_poke);
                        }
                        else
                        {
                            return make_pair(BattleNotification::POKEMON_SWAP, static_cast<FIELD_POSITION>(i));
                        }
                    }
                }

//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_0, 0);
//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_0, 0);
//#if BATTLE_TYPE == DOUBLE_BATTLE
//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_1, 1);
//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_1, 1);
//#endif
//#if BATTLE_TYPE == TRIPLE_BATTLE
//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_2, 2);
//                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_2, 2);
//#endif


                DEBUG_MSG("\n\n\n--------------BATTLE START--------------\n");
                state = BattleState::TURN_START;
                break;
            case BattleState::TURN_START:
                BattleStateMachine::turn_count++;
#ifdef DEBUGGING
                if(BattleStateMachine::turn_count == DEBUG_TURN)
                    DEBUG_MSG("HERERE\n");
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if (BattleStateMachine::battle.active_field.active_pokes[i] != nullptr
                        && BattleStateMachine::battle.active_field.active_pokes[i]->get_current_hp() == 0)
                        ERR_MSG("Pokemon started the turn fainted\n");
                }

                for(int i = 0; i < NUM_PLAYERS; i++)
                {
                    Party* p = battle.get_party(static_cast<Players>(i));
                    for(int j = 0; j < 6; j++)
                    {
                        if(p->party_pokes[j].is_active())
                            break;
                        if(p->party_pokes[j].to_be_swapped)
                            ERR_MSG("Player " << (i+1) << "'s " << p->party_pokes[j].get_species() << " started the turn to be swapped\n");
                        if(j == 5)
                            ERR_MSG("Player " << (i + 1) << " has no active pokes\n");
                    }
                }
                //TODO ADD DOES HAVE ACTIVE POKES?

#endif
                if(BattleStateMachine::turn_count >= MAX_TURN_COUNT)
                    ERR_MSG("Exceeded max turn count\n");
                DEBUG_MSG("\n-------Turn " << BattleStateMachine::turn_count << " start-------\n");
                BattleStateMachine::battle.print_battle();
                state = BattleState::ACTION_REQUEST;

                if(BattleStateMachine::turn_messages.size() != 0)
                    ERR_MSG("Turn Messages was not empty at the start of the turn");

                return make_pair(BattleNotification::POKEMON_ACTION, PLAYER_1_0);

            case BattleState::ACTION_REQUEST:
                //get action choice
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(message.pos == static_cast<FIELD_POSITION>(i))
                    {
                        turn_messages.push_back(message);
                        i++;
                        if(i < FIELD_POSITION::NUM_POSITIONS)
                            return make_pair(BattleNotification::POKEMON_ACTION, static_cast<FIELD_POSITION>(i));
                    }
                }

                //determine attack order
                BattleStateMachine::sort_message_stack();

                message.set_no_op();

                state = BattleState::TURN_EXECUTE;
                break;

            case BattleState::TURN_EXECUTE:
                if(message.move_command != Commands::COMMAND_NONE)
                {
                    BattleStateMachine::turn_messages.push_back(message);
                }

                while(!turn_messages.empty())
                {
                    action_message = turn_messages.back();
                    turn_messages.pop_back();

                    //Determine what attack to do
                    //if there is no attack to execute for that field position, aka it fainted earlier in the turn
                    if(action_message.pos == FIELD_POSITION::NO_POSITION)
                        atk_r = Attack_Result::NO_ATTACK;
                    else if(action_message.move_command == Commands::COMMAND_NONE)
                        continue;
                    //if there needs to be a swap
                    else if(action_message.move_command == Commands::COMMAND_SWAP)
                        atk_r = Attack_Result::SWAP;
                    //default to a attack
                    else if(action_message.move_command == Commands::COMMAND_ATTACK)
                    {
                        atk_r = BattleStateMachine::battle.attack(
                                action_message.pos,
                                action_message.target_pos,
                                action_message.move_num);
                        BattleStateMachine::battle.active_field.active_pokes[action_message.pos]->first_turn = false;
                    }
                    else
                        ERR_MSG("Unhandled action option\n");

                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(action_message.move_command == Commands::COMMAND_ATTACK)
                            {
                                //if attacking and trying to swap an opponent, both the attacker and defender must be alive to execute the swap
                                if((!BattleStateMachine::battle.active_field.position_alive(action_message.pos)
                                 || !BattleStateMachine::battle.active_field.position_alive(action_message.target_pos))
                                 && !BattleStateMachine::battle.active_field.active_pokes[action_message.pos]->moves[action_message.move_num].get_move_effect(-1).does_target_self())
                                    goto swap_end;

                                if(BattleStateMachine::battle.active_field.active_pokes[action_message.pos]->moves[action_message.move_num].get_move_effect(0).does_target_self())
                                {
                                    swap_pos = action_message.pos;
                                }
                                else
                                    swap_pos = action_message.target_pos;

                            }
                            else
                                swap_pos = action_message.pos;

                            if(!BattleStateMachine::battle.can_swap(get_player_from_position(swap_pos)))
                                goto swap_end;

                        // FOR ATTACKING MOVES THAT SWAP
                            if (action_message.move_command == Commands::COMMAND_ATTACK)
                            {
                                // get reserve poke for whatever team needs to swap, if attacking
                                return make_pair(BattleNotification::POKEMON_SWAP, swap_pos);
                            }
                            else
                            {
                                // grab the reserve poke from the battle message if this is a swap action
                                reserve_poke = action_message.reserve_poke;
                            }

                            // TODO: HANDLE PURSUIT HERE

                            //swap pokes!
                            BattleStateMachine::battle.swap_poke(swap_pos, reserve_poke);

                            BattleStateMachine::remove_message_from_stack(swap_pos);

                            swap_end:;

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            for(int p = 0; p < FIELD_POSITION::NUM_POSITIONS; p++)
                            {
                                if (BattleStateMachine::battle.active_field.active_pokes[p] != nullptr
                                && !BattleStateMachine::battle.active_field.active_pokes[p]->is_alive())
                                    faint_player = get_player_from_position(static_cast<FIELD_POSITION>(p));
                                else
                                    goto faint_end;

                                // lose if no pokes left
                                if (battle.has_lost(faint_player))
                                {
                                    goto faint_end;
                                }

                                BattleStateMachine::remove_message_from_stack(static_cast<FIELD_POSITION>(p));

                                if (battle.can_swap(faint_player))
                                    return make_pair(BattleNotification::POKEMON_SWAP, static_cast<FIELD_POSITION>(p));
                                else
                                    BattleStateMachine::battle.swap_poke(static_cast<FIELD_POSITION>(p), -1); //Return the fainted pokemon but do not send out a new pokemon

                                faint_end:;
                            }
                            if(BattleStateMachine::battle_over())
                                return make_pair(BattleNotification::PLAYER_LOST, PLAYER_2_0);
                            break;

                        case Attack_Result::HIT:
                        case Attack_Result::IMMUNE:
                        case Attack_Result::NO_PP:
                        case Attack_Result::MISS:
                        case Attack_Result::NO_ATTACK:
                            break;
                        default:
                            ERR_MSG("Unhandled attack result\n");
                    }

                    if(state != BattleState::TURN_EXECUTE)
                        break;
                }

                if(!turn_messages.empty())
                {
                    ERR_MSG("Attempting to exit from turn execute with " << turn_messages.size() << " messages left to process\n");
                }

                if(state == BattleState::TURN_EXECUTE)
                    state = BattleState::TURN_END_STATUS_CHECK;

                break;
            case BattleState::TURN_END_STATUS_CHECK:
                BattleStateMachine::create_speed_list();
                for(int i =0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(!BattleStateMachine::battle.handle_end_turn_statuses(speed_list.at(i)))
                    {
                        if(BattleStateMachine::battle.has_lost(get_player_from_position(speed_list.at(i))))
                            return make_pair(BattleNotification::PLAYER_LOST, FIELD_POSITION::NO_POSITION);
                    }
                }

                BattleStateMachine::battle.reset_temp_field_status();
                state = BattleState::TURN_END_FAINT_CHECK;
                message.pos = FIELD_POSITION::NO_POSITION;
                break;
            case BattleState::TURN_END_FAINT_CHECK:
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(BattleStateMachine::battle.active_field.active_pokes[i] != nullptr && !BattleStateMachine::battle.active_field.active_pokes[i]->is_alive())
                    {
                        if(message.pos == static_cast<FIELD_POSITION>(i))
                        {
                            if(BattleStateMachine::battle.swap_poke(message.pos, message.reserve_poke) == Attack_Result::FAINT)
                            {
                                if(BattleStateMachine::battle.has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
                                    return make_pair(BattleNotification::PLAYER_LOST, FIELD_POSITION::NO_POSITION);
                            }

                            message.pos = NO_POSITION; // A little hacky.. trying to avoid swapping multiple times with the same message if there are no pokemon left in the party
                            i--;
                        }
                        else
                        {
                            return make_pair(BattleNotification::POKEMON_SWAP, static_cast<FIELD_POSITION>(i));
                        }
                    }
                }

                BattleStateMachine::battle.active_field.handle_end_turn_weather();

                if(BattleStateMachine::battle_over())
                    state = BattleState::BATTLE_END;

                if(state == BattleState::TURN_END_FAINT_CHECK)
                    state = BattleState::TURN_START;
                break;
            case BattleState::BATTLE_END:
                return make_pair(BattleNotification::PLAYER_LOST, FIELD_POSITION::NO_POSITION);
            default:
                ERR_MSG("Unhandled Turn State\n");
        }
    }
}

int BattleStateMachine::get_battle_result()
{
    int loser = 0;
    if(BattleStateMachine::battle.has_lost(Players::PLAYER_ONE) && BattleStateMachine::battle.has_lost(Players::PLAYER_TWO))
        loser = 0;
    else if(BattleStateMachine::battle.has_lost(Players::PLAYER_ONE))
        loser -= 1;
    else if(BattleStateMachine::battle.has_lost(Players::PLAYER_TWO))
        loser += 1;
    else
        ERR_MSG("No loser, can't end battle\n");

    if(loser == 1 || loser == -1)
    {
        DEBUG_MSG("Player " << ((loser + 1) / 2) + 1 << " has lost the battle!\n");
    }
    else if(loser == 0)
    {
        ERR_MSG("TIED on seed" << BattleStateMachine::seed << endl);
    }
    else
        ERR_MSG("How did you get this battle result?\n");

    return loser;
}

// Driver function to sort the 2D vector
// on basis of a particular column
bool sortcol( const vector<int>& v1, const vector<int>& v2 )
{
    return v1[1] > v2[1];
}

void swap_in_place(vector<vector<int>>& v, int src, int dest)
{
    auto temp = v[src];
    v[src] = v[dest];
    v[dest] = temp;
}

void BattleStateMachine::create_speed_list()
{
        vector<vector<int>> speed_map (FIELD_POSITION::NUM_POSITIONS), speed_tie_list;
        vector<FIELD_POSITION> speed_list (FIELD_POSITION::NUM_POSITIONS);
        vector<BattleMessage> temp_messages (FIELD_POSITION::NUM_POSITIONS);

        int prio_choice, map_base;

        for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
        {
            if(BattleStateMachine::battle.active_field.active_pokes[i] != nullptr)
                speed_map.at(i) = {i, static_cast<int>(BattleStateMachine::battle.active_field.active_pokes[i]->get_stat(STAT::SPE))};
            else
                speed_map.at(i) = {i, 0};
        }

        sort(speed_map.begin(), speed_map.end(), sortcol);

        speed_tie_list.push_back(speed_map.front());
        for(unsigned int i = 1; i < speed_map.size(); i++)
        {
            if(speed_map.at(i)[1] == speed_tie_list.back()[1])
            {
                speed_tie_list.push_back(speed_map.at(i));
            }
            else
            {
                for(unsigned int j = 0; j < speed_tie_list.size(); j++)
                {
                    prio_choice = BattleStateMachine::make_choice(speed_tie_list.size() - j);
                    map_base = (i - 1)  - (speed_tie_list.size() - 1) + j;
                    swap_in_place(speed_map, map_base, map_base + prio_choice);
                }
                speed_tie_list.clear();

                speed_tie_list.push_back(speed_map.at(i));

            }
        }
        if(!speed_tie_list.empty())
        {
            for (unsigned int j = 0; j < speed_tie_list.size(); j++)
            {
                prio_choice = BattleStateMachine::make_choice(speed_tie_list.size() - j);
                map_base = speed_map.size() - speed_tie_list.size() + j;
                swap_in_place(speed_map, map_base, map_base + prio_choice);
            }
        }

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        speed_list.at(i) = static_cast<FIELD_POSITION>(speed_map.at(i)[0]);
    }

    BattleStateMachine::speed_list = speed_list;
}

void BattleStateMachine::sort_message_stack()
{
    vector<FIELD_POSITION> prio_list (FIELD_POSITION::NUM_POSITIONS);
    vector<vector<int>> prio_map (FIELD_POSITION::NUM_POSITIONS), speed_tie_list;
    vector<BattleMessage> temp_messages (FIELD_POSITION::NUM_POSITIONS);

    int move_prio, prio_choice, num_messages = BattleStateMachine::turn_messages.size(), map_base;

    for(int i = 0; i < num_messages; i++)
    {
        switch(turn_messages[i].move_command)
        {
            case Commands::COMMAND_SWAP:
                move_prio = (MAX_PRIO + 1) * MAX_SPEED + 1;
                break;
            case Commands::COMMAND_ATTACK:
                move_prio = BattleStateMachine::battle.active_field.active_pokes[turn_messages[i].pos]->moves[turn_messages[i].move_num].get_priority() * MAX_SPEED;
                move_prio += BattleStateMachine::battle.active_field.active_pokes[turn_messages[i].pos]->get_stat(STAT::SPE);
                break;
            case Commands::COMMAND_NONE:
                move_prio = FAINT_PRIO * MAX_SPEED;
                break;
            default:
                ERR_MSG("Unsupported Command " << turn_messages[i].move_command << "\n");
        }
        prio_map.at(turn_messages[i].pos) = {i ,move_prio};
    }

    sort(prio_map.begin(), prio_map.end(), sortcol);
    // Prio map is now sorted by priority and speed, but ties need to be handled randomly


    speed_tie_list.push_back(prio_map.front());
    for(unsigned int i = 1; i < prio_map.size(); i++)
    {
        if(prio_map.at(i)[1] == speed_tie_list.back()[1])
        {
            speed_tie_list.push_back(prio_map.at(i));
        }
        else
        {
            for(unsigned int j = 0; j < speed_tie_list.size(); j++)
            {
                prio_choice = BattleStateMachine::make_choice(speed_tie_list.size() - j);
                map_base = (i - 1)  - (speed_tie_list.size() - 1) + j;
                swap_in_place(prio_map, map_base, map_base + prio_choice);
            }
            speed_tie_list.clear();

            speed_tie_list.push_back(prio_map.at(i));
        }
    }
    if(!speed_tie_list.empty())
    {
        for (unsigned int j = 0; j < speed_tie_list.size(); j++)
        {
            prio_choice = BattleStateMachine::make_choice(speed_tie_list.size() - j);
            map_base = prio_map.size() - speed_tie_list.size() + j;
            swap_in_place(prio_map, map_base, map_base + prio_choice);
        }
    }

    for(unsigned int i = 0; i < turn_messages.size(); i++)
    {
        temp_messages.at(i) = turn_messages.at(prio_map.at(i)[0]);
    }

    turn_messages.clear();
    turn_messages = temp_messages;
    reverse(begin(turn_messages), end(turn_messages));
}

void BattleStateMachine::remove_message_from_stack(FIELD_POSITION pos)
{
    unsigned int i = 0;
    while(i < turn_messages.size())
    {
        if(turn_messages[i].pos == pos)
        {
            turn_messages.erase(turn_messages.begin() + i);
            break;
        }
        i++;
    }
}


Battle* BattleStateMachine::get_battle()
{
    return &battle;
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
    DEBUG_MSG("------RESETTING BATTLE------\n");
    BattleStateMachine::turn_count = 0;
    BattleStateMachine::battle.reset();
    BattleStateMachine::update_random_seed();
    BattleStateMachine::battle.update_generator(BattleStateMachine::get_seed());
    BattleStateMachine::state = BattleState::BATTLE_START;
    BattleStateMachine::turn_messages.clear();
}

int BattleStateMachine::make_choice(int num_positions)
{
    if(num_positions > 6)
        ERR_MSG("Choose position can only support up to 6 choices");

    float c = BattleStateMachine::position_choice(BattleStateMachine::choice);
    return floor(c/(600/num_positions));
}

void BattleStateMachine::validate_battle_message(BattleMessage message)
{

    if(state == BattleState::BATTLE_IDLE)
        return;

    switch(message.move_command)
    {
        case Commands::COMMAND_ATTACK:
            if (state != BattleState::ACTION_REQUEST)
                ERR_MSG("Cannot handle an attack message here\n");
            break;
        case Commands::COMMAND_SWAP:
            if (state != BattleState::ACTION_REQUEST
             && state != BattleState::TURN_EXECUTE
             && state != BattleState::TURN_END_FAINT_CHECK
             && state != BattleState::BATTLE_START)
                ERR_MSG("Cannot handle a swap message here\n");
            break;
        case Commands::COMMAND_TEAM_CHOICE:
            if (state != BattleState::BATTLE_INIT)
                ERR_MSG("Cannot handle a team choice message here\n");
            break;
        case Commands::COMMAND_NONE:
            if (state != BattleState::ACTION_REQUEST
             && state != BattleState::BATTLE_START)
                ERR_MSG("Cannot handle a no op message here\n");
            break;
        default:
            ERR_MSG("Unhandled message type\n");
    }
}