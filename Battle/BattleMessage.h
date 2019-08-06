//
// Created by slist on 6/24/19.
//

#ifndef POKEMON_SIM_BATTLEMESSAGE_H
#define POKEMON_SIM_BATTLEMESSAGE_H

#include <Battle/FieldPositions.h>
#include <string>
using namespace std;

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
    COMMAND_SWAP,
    COMMAND_TEAM_CHOICE,
    COMMAND_MEGA_EVOLVE,
    COMMAND_NONE,
    NUM_COMMANDS
};

enum StateTransition
{
    inactive = 0,
    pending,
    active
};

class BattleMessage
{
public:
    BattleMessage();

    void set_no_op();

    Commands move_command;
    FIELD_POSITION pos;
    bool mega_evolve;

    //Attack fields
    int move_num;
    FIELD_POSITION target_pos;

    //Swap fields
    int reserve_poke;
    FIELD_POSITION active_pos;

    //Team  choice fields
    string team_name;
};


#endif //POKEMON_SIM_BATTLEMESSAGE_H
