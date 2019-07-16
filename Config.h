 //
// Created by slist on 12/17/18.
//

#ifndef POKEMON_SIM_CONFIG_H
#define POKEMON_SIM_CONFIG_H

#include <Battle/BattleConfig.h>

#include <string>
using namespace std;

const string PROJECT_DIRECTORY = "/home/slist/Documents/pokemon-sim/";

#define BATTLE_TYPE DOUBLE_BATTLE
#define MAX_TURN_COUNT 300

#define TEAM_1_CHOICE "team1"
#define TEAM_2_CHOICE "team1"

#define RAND_SEED 2489338927

#define DEBUG_LEVEL 1

#if DEBUG_LEVEL >= 4
#define DEBUGGING
#define DEBUG_TURN 11

#endif

#endif /* POKEMON_SIM_CONFIG_H */
