//
// Created by slist on 12/19/18.
//

#include "BattleActor.h"
#include "Battle.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <Utils/Logging.h>

//BATTLE ACTOR IS CURRENTLY IMPLEMENTED AS AN ALL RANDOM PROCESS. CHOOSING ANYTHING FROM A LIST OF OPTIONS

BattleActor::BattleActor() = default;

BattleActor::BattleActor(long seed)
{
    BattleActor::generator = std::mt19937(seed);
}

BattleMessage::BattleMessage() = default;


BattleMessage BattleActor::choose_action(FIELD_POSITION pos, Party* player_party, Field field, Actions action)
{
    BattleMessage message;
    Players player = get_player_from_position(pos);
    Move move;

    if(action == Actions::CHOOSE_ACTION)
    {
        if(BattleActor::roll_chance(BattleActor::attack_swap_ratio[player]))
        {
            action = Actions::CHOOSE_MOVE;
        }
        else
        {
            action = Actions::CHOOSE_POKEMON;
        }
    }

    switch(action)
    {
        case Actions::CHOOSE_POKEMON:
            message.move_command = Commands::COMMAND_SWAP;
            message.reserve_poke = BattleActor::choose_pokemon(player_party);
            message.active_pos = pos;
            if(message.reserve_poke == -1)
                action = Actions::CHOOSE_MOVE;
            else
                break;;

        case Actions::CHOOSE_MOVE:
            message.move_command = Commands::COMMAND_ATTACK;
            message.move_num = BattleActor::choose_move(field.active_pokes[pos]);
            if(message.move_num != 5)
            {
                move = field.active_pokes[pos]->moves[message.move_num];
                message.target_pos = BattleActor::choose_target(pos, move.get_num_targets(), move.get_move_targets());

            }
            else
            {
                message.target_pos = BattleActor::choose_target(pos, 1, TARGETS::ADJACENT_ENEMY);
            }
            break;;
    }

    DEBUG_MSG("Player " << get_player_from_position(pos) + 1 << " chose action: ");
    if(message.move_command == Commands::COMMAND_SWAP)
    {
        DEBUG_MSG("SWAP, sending out " << player_party->party_pokes[message.reserve_poke].get_species() << "\n");
    }
    else if(message.move_command == Commands::COMMAND_ATTACK)
    {
        DEBUG_MSG("ATTACK, targeting ");
        if(message.target_pos != FIELD_POSITION::ALL_TARGETS)
        {
            DEBUG_MSG("P" << get_player_from_position(message.target_pos) + 1 << "'s ");
            DEBUG_MSG(field.active_pokes[message.target_pos]->get_species());
        }
        else
        {
            DEBUG_MSG("ALL TARGETS");
        }

        DEBUG_MSG(" with ");

        if(message.move_num < 4)
            DEBUG_MSG(field.active_pokes[pos]->moves[message.move_num].get_name() << "\n");
        else
            DEBUG_MSG("struggle\n");
    }
    else
        assert(0);

    return message;
}

int BattleActor::choose_move(Pokemon* poke)
{
    int num_moves = 0, selection;
    int moves [4];
    for(int i = 0; i < 4; i++)
    {
        if(poke->moves[i].get_pp() != 0)
        {
            moves[num_moves] = i;
            num_moves++;
        }
    }

    if(num_moves == 0)
        return 5;

    selection = BattleActor::make_choice(0, num_moves - 1);
    return moves[selection];
}

int BattleActor::choose_pokemon(Party* party)
{
    int num_pokes = 0, selection;
    int pokes [6];
    for(int i = 0; i < 6; i++)
    {
        if(party->party_pokes[i].is_alive() && !party->party_pokes[i].is_active())
        {
            pokes[num_pokes] = i;
            num_pokes++;
        }
    }
    if(num_pokes == 0)
        return -1;

    selection = BattleActor::make_choice(0, num_pokes - 1);
    return pokes[selection];
}

FIELD_POSITION BattleActor::choose_target(FIELD_POSITION atk_pos, int num_targets, TARGETS targets)
{
    if(num_targets == 1)
    {
        BattleActor::actor_targeting.get_valid_targets(targets, atk_pos);
        int rand_target = BattleActor::make_choice(0, BattleActor::actor_targeting.get_num_valid_targets() - 1);
        return BattleActor::actor_targeting.valid_targets[rand_target];
    }
    else
    {
        return FIELD_POSITION::ALL_TARGETS;
    }
}

bool BattleActor::roll_chance(float chance)
{
    float c = std::uniform_real_distribution<float>{0, 1}(BattleActor::generator);
    return c < chance;
}

int BattleActor::make_choice(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(BattleActor::generator);
}

void BattleActor::update_generator(long seed)
{
    BattleActor::generator = std::mt19937(seed);
}