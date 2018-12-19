cmake_minimum_required(VERSION 3.13)
project(pokemon_sim)

set(CMAKE_CXX_STANDARD 14)

include_directories(.
        ../libs/boost)

add_executable(pokemon_sim
        Battle/Battle.cpp
        Battle/Battle.h
        Battle/BattleActor.cpp
        Battle/BattleActor.h
        Battle/BattleStateMachine.cpp
        Battle/BattleStateMachine.h
    Config.h
        Battle/Field.cpp
        Battle/Field.h
        fileIO/loadJSON.cpp
        fileIO/loadJSON.h
    main.cpp
        Pokemon/Move.cpp
        Pokemon/Move.h
        Pokemon/Natures.cpp
        Pokemon/Natures.h
        Battle/Players.h
        Pokemon/Pokemon.cpp
        Pokemon/Pokemon.h
        Pokemon/Stats.h
        Pokemon/Status.cpp
        Pokemon/Status.h
        Pokemon/Type.cpp
        Pokemon/Type.h)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
