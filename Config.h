 //
// Created by slist on 12/17/18.
//

#ifndef POKEMON_SIM_CONFIG_H
#define POKEMON_SIM_CONFIG_H

#include <Battle/BattleConfig.h>

#include <string>

const std::string PROJECT_DIRECTORY = "/home/slist/Documents/pokemon-sim/";

#define BATTLE_TYPE SINGLE_BATTLE
#define DEBUG_LEVEL 0

#if DEBUG_LEVEL > 3
#define DEBUGGING
#endif

#endif /* POKEMON_SIM_CONFIG_H */