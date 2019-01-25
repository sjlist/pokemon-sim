 //
// Created by slist on 12/17/18.
//

#ifndef POKEMON_SIM_CONFIG_H
#define POKEMON_SIM_CONFIG_H

#include <Battle/BattleConfig.h>

#include <string>

const std::string PROJECT_DIRECTORY = "/home/slist/pokemon-sim/";

#define BATTLE_TYPE DOUBLE_BATTLE
#define MAX_TURN_COUNT 300

#define DEBUG_LEVEL 4

#if DEBUG_LEVEL >= 4
#define DEBUGGING
#endif

#endif /* POKEMON_SIM_CONFIG_H */