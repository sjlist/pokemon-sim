//
// Created by slist on 12/19/18.
//

#include "BattleActor.h"
#include "Battle.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>

//BATTLE ACTOR IS CURRENTLY IMPLEMENTED AS AN ALL RANDOM PROCESS. CHOOSING ANYTHING FROM A LIST OF OPTIONS

float attack_swap_ratio [2] = {0.8, 0.8};

BattleActor::BattleActor()
{
    srand (time(NULL));
}

BattleMessage BattleActor::choose_action(FIELD_POSITION pos, Party player_party, Field field, Actions action)
{
    BattleMessage message;
    Players player = get_player_from_position(pos);

    if(action == Actions::CHOOSE_ACTION)
    {
        if(BattleActor::roll_chance(attack_swap_ratio[player]))
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
        case Actions::CHOOSE_MOVE:
            message.move_command = Commands::COMMAND_ATTACK;
            message.move_num = BattleActor::choose_move(field.active_pokes[pos]);
            message.target_pos = BattleActor::choose_target(pos, field.active_pokes[pos].moves[message.move_num]);
            break;;

        case Actions::CHOOSE_POKEMON:
            message.move_command = Commands::COMMAND_SWAP;
            message.reserve_poke = BattleActor::choose_pokemon(player_party);
            message.active_pos = pos;
            break;;
    }

    return message;
}

int BattleActor::choose_move(Pokemon poke)
{
    int choice = rand() % 4;
    while(poke.moves[choice].get_pp() == 0)
    {
        std::cout << "Cannot use " << poke.moves[choice].get_name() << " not enough pp\n";
        choice = rand() % 4;
    }
    return choice;
}

int BattleActor::choose_pokemon(Party party)
{
    int num_pokes = 0, selection;
    int pokes [6];
    for(int i = 0; i < 6; i++)
    {
        if(party.party_pokes[i].is_alive())
        {
            pokes[num_pokes] = i;
            num_pokes++;
        }
    }
    selection = pokes[rand() % num_pokes];
    return selection;
}

FIELD_POSITION BattleActor::choose_target(FIELD_POSITION atk_pos, Move move)
{
    if(move.get_num_targets() == 1)
    {
        BattleActor::actor_targeting.get_valid_targets(move.get_move_targets(), atk_pos);
        int rand_target = rand() % BattleActor::actor_targeting.get_num_valid_targets();
        return BattleActor::actor_targeting.valid_targets[rand_target];
    }
    else
    {
        return FIELD_POSITION::ALL_TARGETS;
    }
}

bool BattleActor::roll_chance(float chance)
{
    float  c = rand()/(float)RAND_MAX;
    return c < chance;
}