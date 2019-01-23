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
                BattleStateMachine::battle.send_out(FIELD_POSITION::PLAYER_2_0, 0);

                DEBUG_MSG("\n\n\n--------------BATTLE START--------------\n");
                state = BattleState::TURN_START;
                break;;
            case BattleState::TURN_START:
                BattleStateMachine::turn_count++;
#ifdef DEBUGGING
                if(BattleStateMachine::turn_count == 40)
                    DEBUG_MSG("HERERE\n");
                for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                    if(BattleStateMachine::battle.active_field.active_pokes[i]->get_current_hp() == 0)
                        ERR_MSG("Pokemon started the turn fainted\n");
#endif
                if(BattleStateMachine::turn_count >= MAX_TURN_COUNT)
                    ERR_MSG("Exceeded max turn count\n");
                DEBUG_MSG("\n-------Turn " << BattleStateMachine::turn_count << " start-------\n");
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
                        ERR_MSG("Unhandled action option\n");

                    switch(atk_r)
                    {
                        case Attack_Result::SWAP:
                            if(messages[prio.at(i)].move_command == Commands::COMMAND_ATTACK)
                            {
                                //if attacking and trying to swap an opponent, both the attacker and defender must be alive to execute the swap
                                if((!BattleStateMachine::battle.active_field.active_pokes[prio.at(i)]->is_alive()
                                 || !BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos]->is_alive())
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
                                while(!BattleStateMachine::battle.swap_poke(swap_pos, reserve_poke))
                                {
                                    // if the poke fainted on entrance

                                    //check if battle is over
                                    if(BattleStateMachine::battle_over())
                                        return BattleStateMachine::end_battle();

                                    // if the battle isnt over, ask for another poke
                                    reserve_poke = BattleStateMachine::actor.choose_pokemon(
                                            BattleStateMachine::battle.get_party(
                                                    get_player_from_position(swap_pos)));
                                }

                                if (messages[prio.at(i)].move_command == Commands::COMMAND_SWAP)
                                    break;;

                                removed = BattleStateMachine::moves_later(swap_pos, i, prio);
                                if (removed != -1)
                                    prio = BattleStateMachine::remove_priority_list(removed, prio);

                                swap_end:;

                                // If both the attacker and defender are alive, no need to handle faints
                                if (BattleStateMachine::battle.active_field.active_pokes[messages[prio.at(i)].target_pos]->is_alive()
                                 && BattleStateMachine::battle.active_field.active_pokes[prio.at(i)]->is_alive())
                                    break;;
                            }

                        case Attack_Result::FAINT:
                            //determine the fainted side
                            for(int p = 0; p < FIELD_POSITION::NUM_POSITIONS; p++)
                            {
                                if(!BattleStateMachine::battle.active_field.active_pokes[p]->is_alive())
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
                if(battle.handle_end_turn_field_status())
                {
                    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
                    {
                        if(!BattleStateMachine::battle.active_field.active_pokes[static_cast<FIELD_POSITION>(i)]->is_alive())
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
                BattleStateMachine::battle.reset_temp_field_status();

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
        ERR_MSG("No loser, can't end battle\n");

    if(loser != 0)
    {
        DEBUG_MSG("Player " << ((loser + 1) / 2) + 1 << " has lost the battle!\n");
    }
    else
    {
        DEBUG_MSG("TIED\n");
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
                move_prio = BattleStateMachine::battle.active_field.active_pokes[pos]->moves[messages[pos].move_num].get_priority();
                break;
            default:
                ERR_MSG("Unsupported Command " << messages[pos].move_command << "\n");
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
        int choice = BattleStateMachine::make_choice(0, 1);
        prio_list.at(0) = static_cast<FIELD_POSITION>(choice);
        prio_list.at(1) = static_cast<FIELD_POSITION>(!choice);
    }
    else
        assert(0);

    return prio_list;
}

// Does th epokemon at pos move later than the current action. If not return, otherwise return the index in the priority list pos takes its action
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
