//
// Created by slist on 12/19/18.
//

#include <Battle/BattleActor.h>
#include <Battle/BattleStateMachine.h>
#include <Battle/Players.h>
#include <Config.h>
#include <fileIO/loadJSON.h>
#include <Pokemon/Move.h>
#include <Utils/Logging.h>

#include <boost/property_tree/ptree.hpp>
#include <cstdlib>
#include <map>
#include <random>

BattleStateMachine::BattleStateMachine()
{
    std::random_device rd;
    BattleStateMachine::seed = rd();
    DEBUG_MSG("The random seed is " << BattleStateMachine::seed << "\n");
    BattleStateMachine::choice = std::mt19937(seed);
    BattleStateMachine::actor = BattleActor(seed);
    BattleStateMachine::battle = Battle(seed);
}

BattleStateMachine::BattleStateMachine(long seed)
{
    BattleStateMachine::seed = seed;
    DEBUG_MSG("The random seed is " << BattleStateMachine::seed << "\n");
    BattleStateMachine::choice = std::mt19937(seed);
    BattleStateMachine::actor = BattleActor(seed);
    BattleStateMachine::battle = Battle(seed);
}

void BattleStateMachine::init()
{
    std::vector<std::string> team_choice(2);
    boost::property_tree::ptree team_tree = load_json_file("team_choice.json");
    team_choice[0] = team_tree.get<std::string>("0");
    team_choice[1] = team_tree.get<std::string>("1");
    BattleStateMachine::battle.load_battle(team_choice);
    //BattleStateMachine::battle.print_battle(true);
}

int BattleStateMachine::run(BattleState state)
{
    Attack_Result atk_r = Attack_Result::NO_ATTACK;
    Players faint_player;
    FIELD_POSITION swap_pos;
    int reserve_poke;
    int removed;

    BattleStateMachine::turn_count = 0;

    BattleMessage messages [FIELD_POSITION::NUM_POSITIONS];
    std::vector<FIELD_POSITION> prio (FIELD_POSITION::NUM_POSITIONS), speed_list (FIELD_POSITION::NUM_POSITIONS);

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

//                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
//                {
//                    BattleStateMachine::battle.send_out(
//                            static_cast<FIELD_POSITION>(i),
//                            actor.choose_pokemon(
//                                    BattleStateMachine::battle.get_party(
//                                            get_player_from_position(i)
//                                            )));
//                }

                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_0, 0);
#if BATTLE_TYPE == DOUBLE_BATTLE
                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_1_1, 1);
#endif
                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_0, 0);
#if BATTLE_TYPE == DOUBLE_BATTLE
                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_1, 1);
#endif

                DEBUG_MSG("\n\n\n--------------BATTLE START--------------\n");
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                BattleStateMachine::turn_count++;
#ifdef DEBUGGING
                if(BattleStateMachine::turn_count == 48)
                    DEBUG_MSG("HERERE\n");
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                    if(BattleStateMachine::battle.active_field.active_pokes[i] != nullptr
                    && BattleStateMachine::battle.active_field.active_pokes[i]->get_current_hp() == 0)
                        ERR_MSG("Pokemon started the turn fainted\n");
#endif
                if(BattleStateMachine::turn_count >= MAX_TURN_COUNT)
                    ERR_MSG("Exceeded max turn count\n");
                DEBUG_MSG("\n-------Turn " << BattleStateMachine::turn_count << " start-------\n");
                BattleStateMachine::battle.print_battle();
                //get action choice
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(BattleStateMachine::battle.active_field.active_pokes[i] != nullptr)
                    {
                        messages[i] = BattleStateMachine::actor.choose_action(
                                static_cast<FIELD_POSITION>(i),
                                BattleStateMachine::battle.get_party(
                                        get_player_from_position(static_cast<FIELD_POSITION>(i))),
                                BattleStateMachine::battle.active_field);
                    }
                    else
                    {
                        messages[i].move_command = Commands::COMMAND_NONE;
                    }
                }

                //determine attack order
                prio = BattleStateMachine::create_priority_list(messages);

                state = BattleState::TURN_EXECUTE;
                break;

            case BattleState::TURN_EXECUTE:
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    //Determine what attack to do
                    //if there is no attack to execute for that field position, aka it fainted earlier in the turn
                    if(prio.at(i) == FIELD_POSITION::NO_POSITION)
                        atk_r = Attack_Result::NO_ATTACK;
                    else if(messages[prio.at(i)].move_command == Commands::COMMAND_NONE)
                        continue;
                    //if there needs to be a swap
                    else if(messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                        atk_r = Attack_Result::SWAP;
                    //default to a attack
                    else if(messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK)
                    {
                        atk_r = BattleStateMachine::battle.attack(
                                prio.at(i),
                                messages[prio.at(i)].target_pos,
                                messages[prio.at(i)].move_num);
                        BattleStateMachine::battle.active_field.active_pokes[prio.at(i)]->first_turn = false;
                    }
                    else
                        ERR_MSG("Unhandled action option\n");

                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK)
                            {
                                //if attacking and trying to swap an opponent, both the attacker and defender must be alive to execute the swap
                                if((!BattleStateMachine::battle.active_field.position_alive(prio.at(i))
                                 || !BattleStateMachine::battle.active_field.position_alive(messages[prio.at(i)].target_pos))
                                 && !BattleStateMachine::battle.active_field.active_pokes[prio.at(i)]->moves[messages[prio.at(i)].move_num].get_move_effect(-1).does_target_self())
                                    goto swap_end;

                                if(BattleStateMachine::battle.active_field.active_pokes[prio.at(i)]->moves[messages[prio.at(i)].move_num].get_move_effect(0).does_target_self())
                                {
                                    swap_pos = prio.at(i);
                                }
                                else
                                    swap_pos = messages[prio.at(i)].target_pos;

                            }
                            else
                                swap_pos = prio.at(i);

                            if(!BattleStateMachine::battle.can_swap(get_player_from_position(swap_pos)))
                            {
                                DEBUG_MSG("No valid pokemon to swap into for player " << (get_player_from_position(swap_pos) + 1) << "\n");
                            }
                            else
                            {
                                // FOR ATTACKING MOVES THAT SWAP
                                if (messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK)
                                {
                                    // get reserve poke for whatever team needs to swap, if attacking
                                    reserve_poke = BattleStateMachine::actor.choose_pokemon(
                                            BattleStateMachine::battle.get_party(
                                                    get_player_from_position(swap_pos)));
                                }
                                else
                                {
                                    // grab the reserve poke from the battle message if this is a swap action
                                    reserve_poke = messages[prio.at(i)].reserve_poke;
                                }

                                // TODO: HANDLE PURSUIT HERE

                                //swap pokes!
                                while(true)
                                {
                                    Attack_Result swap_res = BattleStateMachine::battle.swap_poke(swap_pos, reserve_poke);

                                    // if the poke fainted on entrance
                                    if(swap_res == Attack_Result::FAINT)
                                    {
                                        //check if battle is over
                                        if (BattleStateMachine::battle_over())
                                            return BattleStateMachine::end_battle();

                                        // if the battle isnt over, ask for another poke
                                        reserve_poke = BattleStateMachine::actor.choose_pokemon(
                                                BattleStateMachine::battle.get_party(
                                                        get_player_from_position(swap_pos)));
                                    }
                                    //else if there is no other pokemon to try to send out
                                    else if(swap_res == Attack_Result::NO_ATTACK)
                                    {
                                        break;
                                    }
                                    else if(swap_res == Attack_Result::HIT)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        ERR_MSG("Unhandled Swap Result\n");
                                    }
                                }

                                if (messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                                    break;;

                                removed = BattleStateMachine::moves_later(swap_pos, i, prio);
                                if (removed != -1)
                                    prio = BattleStateMachine::remove_priority_list(removed, prio);

                                swap_end:;

                                // If both the attacker and defender are alive, no need to handle faints
                                if (BattleStateMachine::battle.active_field.position_alive(messages[prio.at(i)].target_pos)
                                 && BattleStateMachine::battle.active_field.position_alive(prio.at(i)))
                                    break;;
                            }

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            for(int p = 0; p < FIELD_POSITION::NUM_POSITIONS; p++)
                            {
                                if(BattleStateMachine::battle.active_field.active_pokes[p] != nullptr
                                && !BattleStateMachine::battle.active_field.active_pokes[p]->is_alive())
                                    faint_player = get_player_from_position(static_cast<FIELD_POSITION>(p));
                                else
                                    goto faint_end;

                                // lose if no pokes left
                                if (battle.has_lost(faint_player)) {
                                    state = BattleState::BATTLE_END;
                                    goto faint_end;
                                }

                                // remove next action in turn if there is one left. use no_player
                                removed = BattleStateMachine::moves_later(static_cast<FIELD_POSITION>(p), i, prio);
                                if (removed != -1)
                                    prio = BattleStateMachine::remove_priority_list(removed, prio);

                                if (BattleStateMachine::battle_over())
                                    state = BattleState::BATTLE_END;
                                else
                                {
                                    //SWAP IN A NEW POKEMON by querying the battle actor
                                    while (true)
                                    {
                                        Attack_Result swap_res = BattleStateMachine::battle.swap_poke(
                                                static_cast<FIELD_POSITION>(p),
                                                BattleStateMachine::actor.choose_pokemon(
                                                        BattleStateMachine::battle.get_party(
                                                                get_player_from_position(
                                                                        static_cast<FIELD_POSITION>(p)))));

                                        // if the poke fainted on entrance
                                        if (swap_res == Attack_Result::FAINT)
                                        {
                                            //check if player has lost
                                            if (BattleStateMachine::battle.has_lost(
                                                    get_player_from_position(static_cast<FIELD_POSITION>(p))))
                                                break;
                                        }
                                            //else if there is no other pokemon to try to send out
                                        else if (swap_res == Attack_Result::NO_ATTACK)
                                        {
                                            break;
                                        }
                                        else if(swap_res == Attack_Result::HIT)
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            ERR_MSG("Unhandled Swap Result\n");
                                        }
                                    }
                                }
                                faint_end:;
                            }
                            if(BattleStateMachine::battle_over())
                                return BattleStateMachine::end_battle();
                            break;;

                        case Attack_Result::FLINCHED:
                            removed = BattleStateMachine::moves_later(messages[prio.at(i)].target_pos, i, prio);
                            if( removed != -1 )
                            {
                                DEBUG_MSG("P" << get_player_from_position(messages[prio.at(i)].target_pos) + 1 << "'s "
                                          << BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos]->get_species() << " flinched\n");

                                prio = BattleStateMachine::remove_priority_list(removed, prio);
                                BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos]->first_turn = false;
                            }
                            break;;

                        case Attack_Result::HIT:
                        case Attack_Result::IMMUNE:
                        case Attack_Result::NO_PP:
                        case Attack_Result::MISS:
                        case Attack_Result::NO_ATTACK:
                            break;;
                        default:
                            ERR_MSG("Unhandled attack result\n");
                    }

                    if(state != BattleState::TURN_EXECUTE)
                        break;
                }

                if(state == BattleState::TURN_EXECUTE)
                    state = BattleState::TURN_END;
                break;;
            case BattleState::TURN_END:
                speed_list = BattleStateMachine::create_speed_list();
                for(int i =0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                {
                    if(!BattleStateMachine::battle.handle_end_turn_statuses(speed_list.at(i)))
                    {
                        if(BattleStateMachine::battle.has_lost(get_player_from_position(speed_list.at(i))))
                            return BattleStateMachine::end_battle();

                        if(!BattleStateMachine::handle_end_turn_swapping(speed_list.at(i)))
                            return BattleStateMachine::end_battle();
                    }
                }
                BattleStateMachine::battle.reset_temp_field_status();

                if(BattleStateMachine::battle_over())
                    state = BattleState::BATTLE_END;

                if(state == BattleState::TURN_END)
                    state = BattleState::TURN_START;
                break;;
            case BattleState::BATTLE_END:
                return BattleStateMachine::end_battle();
            default:
                ERR_MSG("Unhandled Turn State\n");
        }
    }

}

bool BattleStateMachine::handle_end_turn_swapping(FIELD_POSITION pos)
{
    while (true)
    {
        Attack_Result swap_res = BattleStateMachine::battle.swap_poke(
                pos,
                BattleStateMachine::actor.choose_pokemon(
                        BattleStateMachine::battle.get_party(
                                get_player_from_position(
                                        pos))));

        // if the poke fainted on entrance
        if (swap_res == Attack_Result::FAINT)
        {
            //check if player has lost
            if (BattleStateMachine::battle.has_lost(
                    get_player_from_position(pos)))
                return false;
        }
            //else if there is no other pokemon to try to send out
        else if (swap_res == Attack_Result::NO_ATTACK)
        {
            break;
        }
        else if(swap_res == Attack_Result::HIT)
        {
            break;
        }
        else
        {
            ERR_MSG("Unhandled Swap Result\n");
        }
    }

    return true;
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
        ERR_MSG("No loser, can't end battle\n");

    if(loser == 1 || loser == -1)
    {
        DEBUG_MSG("Player " << ((loser + 1) / 2) + 1 << " has lost the battle!\n");
    }
    else if(loser == 0)
    {
        ERR_MSG("TIED\n");
    }
    else
        ERR_MSG("How did you get this battle result?\n");


    return loser;
}

std::vector<FIELD_POSITION> BattleStateMachine::create_speed_list()
{
    std::vector<FIELD_POSITION> speed_list (FIELD_POSITION::NUM_POSITIONS);
    FIELD_POSITION temp;
    int choice;

    // initialize the prio list to be in order of the field positions, just to load the values in
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        speed_list.at(i) = static_cast<FIELD_POSITION>(i);
    }

    //bubble sort by poke speed
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        for(int j = (i + 1); j < FIELD_POSITION::NUM_POSITIONS; j++)
        {
            //if speed of prio i is less than speed of prio j, swap
            // if i is fainted, assume slower even if both are mullptrs
            if(BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i)] == nullptr
            || (BattleStateMachine::battle.active_field.active_pokes[speed_list.at(j)] != nullptr
            && (BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i)]->get_stat(STAT::SPE)
              < BattleStateMachine::battle.active_field.active_pokes[speed_list.at(j)]->get_stat(STAT::SPE))))
            {
                temp = speed_list.at(i);
                speed_list.at(i) = speed_list.at(j);
                speed_list.at(j) = temp;

            }
        }
    }

    //handle speed ties
    //TODO: HANDLE 3 OR 4 WAY TIES CORRECTLY
    for(int i = 0; i < (FIELD_POSITION::NUM_POSITIONS - 1); i++)
    {
        if(BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i)] != nullptr
        && BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i+1)] != nullptr
        && (BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i)]->get_stat(STAT::SPE)
         == BattleStateMachine::battle.active_field.active_pokes[speed_list.at(i+1)]->get_stat(STAT::SPE)))
        {
            choice = BattleStateMachine::make_choice(0, 1);
            if(choice == 1)
            {
                temp = speed_list.at(i);
                speed_list.at(i) = speed_list.at(i+1);
                speed_list.at(i+1) = temp;
            }
        }
    }

    return speed_list;
}

std::vector<FIELD_POSITION> BattleStateMachine::create_priority_list(BattleMessage* messages)
{
    std::vector<FIELD_POSITION> prio_list (FIELD_POSITION::NUM_POSITIONS);
    int move_prio, choice;
    std::vector<int> prio_map (FIELD_POSITION::NUM_POSITIONS);

    for(int pos = FIELD_POSITION::PLAYER_1_0; pos < FIELD_POSITION::NUM_POSITIONS; pos++)
    {
        switch(messages[pos].move_command)
        {
            case Commands::COMMAND_SWAP:
                move_prio = MAX_PRIO + 1;
                break;
            case Commands::COMMAND_ATTACK:
                move_prio = BattleStateMachine::battle.active_field.active_pokes[pos]->moves[messages[pos].move_num].get_priority();
                break;
            case Commands::COMMAND_NONE:
                move_prio = FAINT_PRIO;
                break;
            default:
                ERR_MSG("Unsupported Command " << messages[pos].move_command << "\n");
        }
        prio_map.at(pos) = move_prio;
    }
#if BATTLE_TYPE == SINGLE_BATTLE
    if(prio_map.at(FIELD_POSITION::PLAYER_1_0) > prio_map.at(FIELD_POSITION::PLAYER_2_0))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_2_0;
    }
    else if(prio_map.at(FIELD_POSITION::PLAYER_1_0) < prio_map.at(FIELD_POSITION::PLAYER_2_0))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_1_0]->get_stat(STAT::SPE)
          > BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_2_0]->get_stat(STAT::SPE))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_1_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_2_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_1_0]->get_stat(STAT::SPE)
          < BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_2_0]->get_stat(STAT::SPE))
    {
        prio_list.at(0) = FIELD_POSITION::PLAYER_2_0;
        prio_list.at(1) = FIELD_POSITION::PLAYER_1_0;
    }
    else if(BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_1_0]->get_stat(STAT::SPE)
         == BattleStateMachine::battle.active_field.active_pokes[FIELD_POSITION::PLAYER_2_0]->get_stat(STAT::SPE))
    {
        choice = BattleStateMachine::make_choice(0, 1);
        prio_list.at(0) = static_cast<FIELD_POSITION>(choice);
        prio_list.at(1) = static_cast<FIELD_POSITION>(!choice);
    }
    else
        ERR_MSG("Unhandled Priority list case\n");
#else
    FIELD_POSITION temp;

    // initialize the prio list to be in order of the field positions, just to load the values in
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        prio_list.at(i) = static_cast<FIELD_POSITION>(i);
    }

    //bubble sort by move priority
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        for(int j = (i + 1); j < FIELD_POSITION::NUM_POSITIONS; j++)
        {
            //if i prio is less than j prio, swap
            if(prio_map.at(prio_list.at(i)) < prio_map.at(prio_list.at(j)))
            {
                temp = prio_list.at(i);
                prio_list.at(i) = prio_list.at(j);
                prio_list.at(j) = temp;

            }
        }
    }

    //bubble sort by speed within move priority
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        for(int j = (i + 1); j < FIELD_POSITION::NUM_POSITIONS; j++)
        {
            //if i prio is equal to j prio, compare speeds, if slower then swap
            //but skip this check if both pokes are fainted
            if(prio_map.at(prio_list.at(i)) == prio_map.at(prio_list.at(j))
            && prio_map.at(prio_list.at(i)) != FAINT_PRIO
            && BattleStateMachine::battle.active_field.active_pokes[prio_list.at(i)]->get_stat(STAT::SPE)
             < BattleStateMachine::battle.active_field.active_pokes[prio_list.at(j)]->get_stat(STAT::SPE))
            {
                temp = prio_list.at(i);
                prio_list.at(i) = prio_list.at(j);
                prio_list.at(j) = temp;

            }
        }
    }

    //handle speed ties
    //TODO: HANDLE 3 OR 4 WAY TIES CORRECTLY
    for(int i = 0; i < (FIELD_POSITION::NUM_POSITIONS - 1); i++)
    {
        if(prio_map.at(prio_list.at(i)) == prio_map.at(prio_list.at(i+1))
        && prio_map.at(prio_list.at(i)) != FAINT_PRIO
        && (BattleStateMachine::battle.active_field.active_pokes[prio_list.at(i)]->get_stat(STAT::SPE)
         == BattleStateMachine::battle.active_field.active_pokes[prio_list.at(i+1)]->get_stat(STAT::SPE)))
        {
            choice = BattleStateMachine::make_choice(0, 1);
            if(choice == 1)
            {
                temp = prio_list.at(i);
                prio_list.at(i) = prio_list.at(i+1);
                prio_list.at(i+1) = temp;
            }
        }
    }
#endif

    return prio_list;
}

// Does the pokemon at pos move later than the current action. If not return, otherwise return the index in the priority list pos takes its action
int BattleStateMachine::moves_later(FIELD_POSITION pos, int current_action, std::vector<FIELD_POSITION> prio_list)
{
    for(int j = (current_action + 1); j < FIELD_POSITION::NUM_POSITIONS; j++)
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
    DEBUG_MSG("------RESETTING BATTLE------\n");
    std::random_device rd;
    BattleStateMachine::seed = rd();
    DEBUG_MSG("New random seed: " << seed << std::endl);
    BattleStateMachine::turn_count = 0;
    BattleStateMachine::battle.reset();
    BattleStateMachine::battle.update_generator(seed);
    BattleStateMachine::actor.update_generator(seed);
}

int BattleStateMachine::make_choice(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(BattleStateMachine::choice);
}
