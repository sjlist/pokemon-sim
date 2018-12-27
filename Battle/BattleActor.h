//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLEACTOR_H
#define POKEMON_SIM_BATTLEACTOR_H

#include "Players.h"
#include "Field.h"

enum class Actions
{
    CHOOSE_POKEMON = 0,
    CHOOSE_MOVE,
    CHOOSE_TARGET,
    CHOOSE_ACTION,
    NUM_ACTIONS
};

enum Commands
{
    COMMAND_ATTACK = 0,
    COMMAND_SWAP
};

class BattleActor
{
public:
    BattleActor();

    int choose_action();
    int choose_pokemon();
    int choose_target();
    int choose_move();
};

class BattleMessage
{
public:
    Commands move_command;

    //Attack fields
    int move_num;
    FIELD_POSITION target_pos;

    //Swap commands
    int reserve_poke;
    FIELD_POSITION active_pos;
};
#endif //POKEMON_SIM_BATTLEACTOR_H
