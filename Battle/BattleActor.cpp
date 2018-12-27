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

BattleMessage BattleActor::choose_action(Players player, Battle battle, Actions action)
{
    BattleMessage message;

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
            message.move_num = BattleActor::choose_move();
            message.target_pos = BattleActor::choose_target(player);
            break;;

        case Actions::CHOOSE_POKEMON:
            message.move_command = Commands::COMMAND_SWAP;
            message.reserve_poke = BattleActor::choose_pokemon(battle.get_party(player));
            message.active_pos = BattleActor::get_active(player);
            break;;
    }

    return message;
}

int BattleActor::choose_move()
{
    return rand() % 4;
}

int BattleActor::choose_pokemon(Party party)
{
    int num_pokes = 0;
    int pokes [6];
    for(int i = 0; i < 6; i++)
    {
        if(party.party_pokes[i].is_alive())
        {
            pokes[num_pokes] = i;
            num_pokes++;
        }
    }

    return pokes[rand() % num_pokes];
}

FIELD_POSITION BattleActor::choose_target(Players player)
{
    if(player == Players::PLAYER_ONE)
    {
        return FIELD_POSITION::PLAYER_2_0;
    }

    if(player == Players::PLAYER_TWO)
        return FIELD_POSITION::PLAYER_1_0;
}

FIELD_POSITION BattleActor::get_active(Players player)
{
    if(player == Players::PLAYER_ONE)
    {
        return FIELD_POSITION::PLAYER_1_0;
    }

    if(player == Players::PLAYER_TWO)
        return FIELD_POSITION::PLAYER_2_0;
}


bool BattleActor::roll_chance(float chance)
{
    float  c = rand()/(float)RAND_MAX;
    return c < chance;
}