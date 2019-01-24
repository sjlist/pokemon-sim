//
// Created by slist on 1/16/19.
//

#ifndef POKEMON_SIM_LOGGING_H
#define POKEMON_SIM_LOGGING_H

#include <Config.h>

#include <cassert>
#include <iostream>

#if DEBUG_LEVEL >= 3
#define DEBUG_MESSAGES
#endif

#if DEBUG_LEVEL >= 2
#define WARNING_MESSAGES
#endif

#if DEBUG_LEVEL >= 1
#define ERROR_MESSAGES
#endif

#ifdef DEBUG_MESSAGES
#define DEBUG_MSG(str) do { std::cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


#ifdef WARNING_MESSAGES
#define WARN_MSG(str) do { std::cout << str; } while( false )
#else
#define WARN_MSG(str) do { } while ( false )
#endif

#ifdef ERROR_MESSAGES
#define ERR_MSG(str) do { std::cout << str;  assert(0); } while( false )
#else
#define ERR_MSG(str) do { assert(0); } while ( false )
#endif

#endif //POKEMON_SIM_LOGGING_H
