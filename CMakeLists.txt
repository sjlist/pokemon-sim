cmake_minimum_required(VERSION 3.13)
project(pokemon_sim)

set(CMAKE_CXX_STANDARD 14)

include_directories(.
    ../libs/boost)

add_executable(pokemon_sim
    Battle.cpp
    Battle.h
    Config.h
    Field.cpp
    Field.h
    loadJSON.cpp
    loadJSON.h
    main.cpp
    Move.cpp
    Move.h
    Natures.cpp
    Natures.h
    Players.h
    Pokemon.cpp
    Pokemon.h
    Stats.h
    Status.cpp
    Status.h
    Type.cpp
    Type.h)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
