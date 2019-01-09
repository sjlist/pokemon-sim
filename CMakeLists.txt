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
        Battle/BattleConfig.h
        Battle/BattleStateMachine.cpp
        Battle/BattleStateMachine.h
        Battle/Field.cpp
        Battle/Field.h
        Battle/Field_Positions.h
        Battle/Field_Objects.cpp
        Battle/Field_Objects.h
        Battle/Party.cpp
        Battle/Party.h
        Battle/Players.h
        Battle/Targeting.cpp
        Battle/Targeting.h
    Config.h
        fileIO/loadJSON.cpp
        fileIO/loadJSON.h
    main.cpp
        Pokemon/Effect.cpp
        Pokemon/Effect.h
        Pokemon/Move.cpp
        Pokemon/Move.h
        Pokemon/Natures.cpp
        Pokemon/Natures.h
        Pokemon/Pokemon.cpp
        Pokemon/Pokemon.h
        Pokemon/Stats.cpp
        Pokemon/Stats.h
        Pokemon/Status.cpp
        Pokemon/Status.h
        Pokemon/Type.cpp
        Pokemon/Type.h)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
