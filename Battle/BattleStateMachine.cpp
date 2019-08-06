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
    seed = rd();
    DEBUG_MSG("The random seed is " << seed << "\n");
    choice = mt19937(seed);
    position_choice = uniform_int_distribution<int> {0, 599};
    battle = Battle(seed);
    state = BattleState::BATTLE_IDLE;

    turn_count = 0;
}

void BattleStateMachine::init(Players player, string* team_name)
{
    battle.load_battle(player, team_name);
}

void BattleStateMachine::update_random_seed(long new_seed)
{
    seed = new_seed;
    DEBUG_MSG("The random seed is " << seed << "\n");
    choice = mt19937(seed);
    battle.update_generator(seed);
}
void BattleStateMachine::update_random_seed()
{
    random_device rd;
    seed = rd();
    DEBUG_MSG("The random seed is " << seed << "\n");
    choice = mt19937(seed);
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
                init(get_player_from_position(message.pos), &message.team_name);
                DEBUG_MSG("Loaded Player " << get_string_from_field_position(message.pos) << endl);
                if(battle.get_party(PLAYER_ONE)->loaded && battle.get_party(PLAYER_TWO)->loaded)
                {
                    state = BattleState::BATTLE_START;
                    //battle.print_battle();
                    break;
                }
                else if(!battle.get_party(PLAYER_ONE)->loaded)
                    return make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
                else if(!battle.get_party(PLAYER_TWO)->loaded)
                    return make_pair(BattleNotification::TEAM_CHOICE, PLAYER_2_0);
                else
                    ERR_MSG("UNREACHABLE during team loading");

            case BattleState::BATTLE_START:
                turn_count = 0;

                //get pokemon to send out

                for(int i = 0; i < NUM_POSITIONS; i++)
                {
                    if(battle.active_field.active_pokes[i] == nullptr)
                    {
                        if (message.pos == static_cast<FIELD_POSITION>(i))
                        {
                            battle.send_out(static_cast<FIELD_POSITION>(i), message.reserve_poke);
                        }
                        else
                        {
                            return make_pair(BattleNotification::POKEMON_SWAP, static_cast<FIELD_POSITION>(i));
                        }
                    }
                }

                DEBUG_MSG("\n\n\n--------------BATTLE START--------------\n");
                state = BattleState::TURN_START;
                break;
            case BattleState::TURN_START:
                turn_count++;
                DEBUG_MSG("\n-------Turn " << turn_count << " start-------\n");
                battle.print_battle();
#ifdef DEBUGGING
                if(turn_count == DEBUG_TURN)
                    DEBUG_MSG("HERERE\n");
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if (battle.active_field.active_pokes[i] != nullptr
                        && battle.active_field.active_pokes[i]->get_current_hp() == 0)
                        ERR_MSG("Pokemon started the turn fainted\n");
                }

                for(int i = 0; i < NUM_PLAYERS; i++)
                {
                    Party* p = battle.get_party(static_cast<Players>(i));
                    int num_party_pokes_alive = 0, num_active_pokes = 0;
                    for(int j = 0; j < 6; j++)
                    {
                        if(p->party_pokes[j].is_active())
                            num_active_pokes++;
                        if(p->party_pokes[j].is_alive())
                            num_party_pokes_alive++;
                        if(p->party_pokes[j].to_be_swapped)
                            ERR_MSG("Player " << (i+1) << "'s " << p->party_pokes[j].get_species() << " started the turn to be swapped\n");
                    }
                    if((num_party_pokes_alive > num_active_pokes) && (num_active_pokes < (NUM_POSITIONS / 2)))
                        ERR_MSG("Player " << (i+1) << " started the turn with an empty position but a pokemon that could fill it in the party" << endl);
                }
                //TODO ADD DOES HAVE ACTIVE POKES?

#endif
                if(turn_count >= MAX_TURN_COUNT)
                    ERR_MSG("Exceeded max turn count\n");
                state = BattleState::ACTION_REQUEST;

                if(!turn_messages.empty())
                    ERR_MSG("Turn Messages was not empty at the start of the turn\n");

                return make_pair(BattleNotification::POKEMON_ACTION, PLAYER_1_0);

            case BattleState::ACTION_REQUEST:
                //get action choice
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(message.pos == static_cast<FIELD_POSITION>(i))
                    {
                        if(message.mega_evolve)
                        {
                            mega_messages.push_back(get_mega_message(message.pos));
                            battle.mega_pending(message.pos);
                        }
                        else
                            mega_messages.push_back(get_no_op_message(message.pos));

                        turn_messages.push_back(message);
                        i++;
                        if(i < FIELD_POSITION::NUM_POSITIONS)
                            return make_pair(BattleNotification::POKEMON_ACTION, static_cast<FIELD_POSITION>(i));
                    }
                }

                //determine attack order
                sort_message_stack(&turn_messages);
                sort_message_stack(&mega_messages);

                for(unsigned int i = 0; i < mega_messages.size(); i++)
                {
                    if(mega_messages[i].move_command == Commands::COMMAND_MEGA_EVOLVE)
                        turn_messages.push_back(mega_messages[i]);
                }

                mega_messages.clear();
                message.set_no_op();

                state = BattleState::TURN_EXECUTE;
                break;

            case BattleState::TURN_EXECUTE:
                if(message.move_command != Commands::COMMAND_NONE)
                {
                    turn_messages.push_back(message);
                }

                while(!turn_messages.empty())
                {
                    action_message = turn_messages.back();
                    turn_messages.pop_back();

                    //Determine what attack to do
                    switch(action_message.move_command)
                    {
                        case Commands::COMMAND_MEGA_EVOLVE:
                            DEBUG_MSG("Mega evolving P" << get_player_from_position(action_message.pos) << "'s " << battle.active_field.active_pokes[action_message.pos]->get_species() << endl);
                            battle.mega_evolve(action_message.pos);
                            atk_r = Attack_Result::NO_ATTACK;
                            break;
                        case Commands::COMMAND_SWAP:
                            atk_r = Attack_Result::SWAP;
                            break;
                        case Commands::COMMAND_NONE:
                            atk_r = Attack_Result::NO_ATTACK;
                            break;
                        case Commands::COMMAND_ATTACK:
                            atk_r = battle.attack(
                                    action_message.pos,
                                    action_message.target_pos,
                                    action_message.move_num);
                            battle.active_field.active_pokes[action_message.pos]->first_turn = false;
                            break;
                        default:
                            ERR_MSG("Unhandled action option\n");
                    }

                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(action_message.move_command == Commands::COMMAND_ATTACK)
                            {
                                //if attacking and trying to swap an opponent, both the attacker and defender must be alive to execute the swap
                                if((!battle.active_field.position_alive(action_message.pos)
                                 || !battle.active_field.position_alive(action_message.target_pos))
                                 && !battle.active_field.active_pokes[action_message.pos]->moves[action_message.move_num].get_move_effect(-1).does_target_self())
                                    goto swap_end;

                                if(battle.has_lost(get_player_from_position(action_message.target_pos)))
                                    return make_pair(BattleNotification::PLAYER_LOST, action_message.target_pos);

                                if(battle.active_field.active_pokes[action_message.pos]->moves[action_message.move_num].get_move_effect(0).does_target_self())
                                {
                                    swap_pos = action_message.pos;
                                }
                                else
                                    swap_pos = action_message.target_pos;

                            }
                            else
                                swap_pos = action_message.pos;

                            if(!battle.can_swap(get_player_from_position(swap_pos)))
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
                            if(battle.swap_poke(swap_pos, reserve_poke) == Attack_Result::FAINT)
                            {
                                if(battle.has_lost(get_player_from_position(swap_pos)))
                                    return make_pair(BattleNotification::PLAYER_LOST, swap_pos);
                                if(battle.can_swap(get_player_from_position(swap_pos)))
                                    return make_pair(BattleNotification::POKEMON_SWAP, swap_pos);
                            }

                            remove_message_from_stack(swap_pos);

                            swap_end:;

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            for(int p = 0; p < FIELD_POSITION::NUM_POSITIONS; p++)
                            {
                                if (battle.active_field.active_pokes[p] != nullptr
                                && !battle.active_field.active_pokes[p]->is_alive())
                                    faint_player = get_player_from_position(static_cast<FIELD_POSITION>(p));
                                else
                                    goto faint_end;

                                // lose if no pokes left
                                if (battle.has_lost(faint_player))
                                {
                                    goto faint_end;
                                }

                                remove_message_from_stack(static_cast<FIELD_POSITION>(p));

                                if (battle.can_swap(faint_player))
                                    return make_pair(BattleNotification::POKEMON_SWAP, static_cast<FIELD_POSITION>(p));
                                else
                                    battle.swap_poke(static_cast<FIELD_POSITION>(p), -1); //Return the fainted pokemon but do not send out a new pokemon

                                faint_end:;
                            }
                            if(battle_over())
                                return make_pair(BattleNotification::PLAYER_LOST, FIELD_POSITION::NO_POSITION);
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
                create_speed_list();
                for(int i =0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(!battle.handle_end_turn_statuses(speed_list.at(i)))
                    {
                        if(battle.has_lost(get_player_from_position(speed_list.at(i))))
                            return make_pair(BattleNotification::PLAYER_LOST, FIELD_POSITION::NO_POSITION);
                    }
                }

                battle.reset_temp_field_status();
                state = BattleState::TURN_END_FAINT_CHECK;
                message.pos = FIELD_POSITION::NO_POSITION;
                break;
            case BattleState::TURN_END_FAINT_CHECK:
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(battle.active_field.active_pokes[i] != nullptr && !battle.active_field.active_pokes[i]->is_alive())
                    {
                        if(message.pos == static_cast<FIELD_POSITION>(i))
                        {
                            if(battle.swap_poke(message.pos, message.reserve_poke) == Attack_Result::FAINT)
                            {
                                if(battle.has_lost(get_player_from_position(static_cast<FIELD_POSITION>(i))))
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

                battle.active_field.handle_end_turn_weather();

                if(battle_over())
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
    if(battle.has_lost(Players::PLAYER_ONE) && battle.has_lost(Players::PLAYER_TWO))
        loser = 0;
    else if(battle.has_lost(Players::PLAYER_ONE))
        loser -= 1;
    else if(battle.has_lost(Players::PLAYER_TWO))
        loser += 1;
    else
        ERR_MSG("No loser, can't end battle\n");

    if(loser == 1 || loser == -1)
    {
        DEBUG_MSG("Player " << ((loser + 1) / 2) + 1 << " has lost the battle!\n");
    }
    else if(loser == 0)
    {
        ERR_MSG("TIED on seed " << seed << endl);
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
        vector<FIELD_POSITION> temp_speed_list (FIELD_POSITION::NUM_POSITIONS);
        vector<BattleMessage> temp_messages (FIELD_POSITION::NUM_POSITIONS);

        int prio_choice, map_base;

        for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
        {
            if(battle.active_field.active_pokes[i] != nullptr)
                speed_map.at(i) = {i, static_cast<int>(battle.active_field.active_pokes[i]->get_stat(STAT::SPE))};
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
                    prio_choice = make_choice(speed_tie_list.size() - j);
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
                prio_choice = make_choice(speed_tie_list.size() - j);
                map_base = speed_map.size() - speed_tie_list.size() + j;
                swap_in_place(speed_map, map_base, map_base + prio_choice);
            }
        }

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        temp_speed_list.at(i) = static_cast<FIELD_POSITION>(speed_map.at(i)[0]);
    }

    speed_list = temp_speed_list;
}

void BattleStateMachine::sort_message_stack(vector<BattleMessage>*  messages)
{
    vector<FIELD_POSITION> prio_list (messages->size());
    vector<vector<int>> prio_map (messages->size()), speed_tie_list;
    vector<BattleMessage> temp_messages (messages->size());

    int move_prio = 0, prio_choice, num_messages = messages->size(), map_base;

    for(int i = 0; i < num_messages; i++)
    {
        switch(messages->at(i).move_command)
        {
            case Commands::COMMAND_SWAP:
                move_prio = SWAP_PRIO * MAX_SPEED + 1;
                move_prio += static_cast<int>(battle.active_field.active_pokes[messages->at(i).pos]->get_stat(STAT::SPE));
                break;
            case Commands::COMMAND_ATTACK:
                move_prio = battle.active_field.active_pokes[messages->at(i).pos]->moves[messages->at(i).move_num].get_priority() * MAX_SPEED;
                move_prio += static_cast<int>(battle.active_field.active_pokes[messages->at(i).pos]->get_stat(STAT::SPE));
                break;
            case Commands::COMMAND_NONE:
                move_prio = FAINT_PRIO * MAX_SPEED;
                break;
            case Commands::COMMAND_MEGA_EVOLVE:
                move_prio = MEGA_EVOLVE_PRIO * MAX_SPEED + 1;
                move_prio += static_cast<int>(battle.active_field.active_pokes[messages->at(i).pos]->get_stat(STAT::SPE));
                break;
            default:
                ERR_MSG("Unsupported Command " << messages->at(i).move_command << "\n");
        }
        prio_map.at(messages->at(i).pos) = {i ,move_prio};
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
                prio_choice = make_choice(speed_tie_list.size() - j);
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
            prio_choice = make_choice(speed_tie_list.size() - j);
            map_base = prio_map.size() - speed_tie_list.size() + j;
            swap_in_place(prio_map, map_base, map_base + prio_choice);
        }
    }

    for(unsigned int i = 0; i < messages->size(); i++)
    {
        temp_messages.at(i) = messages->at(prio_map.at(i)[0]);
    }

    messages->clear();
    reverse(begin(temp_messages), end(temp_messages));
    for(unsigned int i = 0; i < temp_messages.size(); i++)
        messages->push_back(temp_messages[i]);
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
    return turn_count;
}

long BattleStateMachine::get_seed()
{
    return seed;
}

bool BattleStateMachine::battle_over()
{
    return battle.has_lost(Players::PLAYER_ONE) || battle.has_lost(Players::PLAYER_TWO);
}

void BattleStateMachine::reset()
{
    DEBUG_MSG("------RESETTING BATTLE------\n");
    turn_count = 0;
    battle.reset();
    update_random_seed();
    battle.update_generator(get_seed());
    state = BattleState::BATTLE_START;
    turn_messages.clear();
}

int BattleStateMachine::make_choice(int num_positions)
{
    if(num_positions > 6)
        ERR_MSG("Choose position can only support up to 6 choices");

    float c = position_choice(choice);
    return floor(c/(static_cast<float>(600)/num_positions));
}

BattleMessage BattleStateMachine::get_mega_message(FIELD_POSITION pos)
{
    BattleMessage m;
    m.move_command = Commands::COMMAND_MEGA_EVOLVE;
    m.pos = pos;
    return m;
}

BattleMessage BattleStateMachine::get_no_op_message(FIELD_POSITION pos)
{
    BattleMessage m;
    m.move_command = Commands::COMMAND_NONE;
    m.pos = pos;
    return m;
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