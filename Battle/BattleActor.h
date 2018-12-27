//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLEACTOR_H
#define POKEMON_SIM_BATTLEACTOR_H

#include "Players.h"
#include "Field.h"
#include "Battle.h"

enum class Actions
{
    CHOOSE_POKEMON = 0,
    CHOOSE_MOVE,
    CHOOSE_ACTION,
    NUM_ACTIONS
};

enum Commands
{
    COMMAND_ATTACK = 0,
    COMMAND_SWAP
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

class BattleActor
{
public:
    BattleActor();

    BattleMessage choose_action(Players player, Battle battle, Actions action=Actions::CHOOSE_ACTION);

private:
    FIELD_POSITION get_active(Players player);
    FIELD_POSITION choose_target(Players player);
    int choose_pokemon(Party party);
    int choose_move();

    bool roll_chance(float chance);


};
#endif //POKEMON_SIM_BATTLEACTOR_H
