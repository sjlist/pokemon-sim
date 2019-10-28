 //
// Created by slist on 12/17/18.
//

#ifndef POKEMON_SIM_CONFIG_H
#define POKEMON_SIM_CONFIG_H

#include <Battle/BattleConfig.h>

#include <string>
using namespace std;

const string PROJECT_DIRECTORY = "/home/slist/projects/pokemon-sim/";

#define BATTLE_TYPE DOUBLE_BATTLE
#define MAX_TURN_COUNT 400

#define RAND_SEED 1758096715

#define DEBUG_LEVEL 1

#if DEBUG_LEVEL >= 4
#define DEBUGGING
#define DEBUG_TURN 25
#endif

#endif /* POKEMON_SIM_CONFIG_H */
