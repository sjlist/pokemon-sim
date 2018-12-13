cmake_minimum_required(VERSION 3.13)
project(pokemon_sim)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(pokemon_sim
    main.cpp
    Move.cpp
    Move.h
    Pokemon.cpp
    Pokemon.h
    Type.cpp
    Type.h)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
