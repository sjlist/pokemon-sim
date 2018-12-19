//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLEACTOR_H
#define POKEMON_SIM_BATTLEACTOR_H

enum class Actions
{
    CHOOSE_POKEMON = 0,
    CHOOSE_MOVE,
    CHOOSE_TARGET,
    CHOOSE_ACTION,
    NUM_ACTIONS
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


#endif //POKEMON_SIM_BATTLEACTOR_H
