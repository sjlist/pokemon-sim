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
    BattleMessage();
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
    BattleActor(long seed);

    float attack_swap_ratio [2] = {0.8, 0.8};

    int choose_pokemon(Party* party);
    BattleMessage choose_action(FIELD_POSITION atk_pos, Party* player_party, Field field, Actions action=Actions::CHOOSE_ACTION);
    void update_generator(long seed);
private:
    std::mt19937 generator;
    Targets actor_targeting;

    int make_choice(int min, int max);

    FIELD_POSITION choose_target(FIELD_POSITION atk_pos, int num_moves, TARGETS targets);
    int choose_move(Pokemon* poke);

    bool roll_chance(float chance);
};
#endif //POKEMON_SIM_BATTLEACTOR_H
