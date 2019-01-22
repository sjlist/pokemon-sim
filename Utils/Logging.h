//
// Created by slist on 1/16/19.
//

#ifndef POKEMON_SIM_LOGGING_H
#define POKEMON_SIM_LOGGING_H

#include <Config.h>

#include <iostream>

#ifdef DEBUG_MESSAGES
#define DEBUG_MSG(str) do { std::cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#endif //POKEMON_SIM_LOGGING_H
