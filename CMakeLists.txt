cmake_minimum_required(VERSION 3.13)
project(pokemon_sim)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

##################################################
### Source
##################################################
set(BOOST_VERSION 1.70.0)
find_package(Boost ${BOOST_VERSION})

include_directories(.
        ${Boost_INCLUDE_DIRS})

set(BATTLE_FILES
        Battle/Battle.cpp
        Battle/Battle.h
        Battle/BattleActor.cpp
        Battle/BattleActor.h
        Battle/BattleConfig.h
        Battle/BattleMessage.cpp
        Battle/BattleMessage.h
        Battle/BattleStateMachine.cpp
        Battle/BattleStateMachine.h
        Battle/Field.cpp
        Battle/Field.h
        Battle/FieldPositions.cpp
        Battle/FieldPositions.h
        Battle/FieldObjects.cpp
        Battle/FieldObjects.h
        Battle/Party.cpp
        Battle/Party.h
        Battle/Players.h
        Battle/Targeting.cpp
        Battle/Targeting.h)

set(POKEMON_FILES
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

set(FILE_IO_FILES
        fileIO/loadJSON.cpp
        fileIO/loadJSON.h)

set(UTIL_FILES
        Utils/Logging.h)

set(POKEMON_SIM_FILES
        ${POKEMON_FILES}
        ${FILE_IO_FILES}
        ${UTIL_FILES}
        ${BATTLE_FILES}
        Config.h)

add_library(Pokemon_Battle_Sim SHARED ${POKEMON_SIM_FILES})

add_executable(pokemon_sim
        Player_Config.h
        main.cpp)

target_link_libraries(pokemon_sim gtest_main Pokemon_Battle_Sim)

##################################################
### GTEST
##################################################
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

##################################################
### UNIT TESTS
##################################################
set(BATTLE_TEST_FILES
        Battle/unit-tests/probability-helpers.h
        Battle/unit-tests/probability-helpers.cpp
        Battle/unit-tests/test-Field.cpp
        Battle/unit-tests/test-Targeting.cpp
        Battle/unit-tests/test-Battle.cpp
        Battle/unit-tests/test-BattleActor.cpp
        Battle/unit-tests/test-BattleStateMachine.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-all
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-all gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-all COMMAND unit-test-all)

set(BATTLE_TEST_FILES
        Battle/unit-tests/test-Field.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-Field
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-Field gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-Field COMMAND unit-test-Field)

set(BATTLE_TEST_FILES
        Battle/unit-tests/test-Targeting.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-Targeting
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-Targeting gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-Targeting COMMAND unit-test-Targeting)

set(BATTLE_TEST_FILES
        Battle/unit-tests/test-Battle.cpp
        Battle/unit-tests/probability-helpers.h
        Battle/unit-tests/probability-helpers.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-Battle
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-Battle gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-Battle COMMAND unit-test-Battle)

set(BATTLE_TEST_FILES
        Battle/unit-tests/test-BattleActor.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-BattleActor
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-BattleActor gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-BattleActor COMMAND unit-test-BattleActor)

set(BATTLE_TEST_FILES
        Battle/unit-tests/test-BattleStateMachine.cpp
        Battle/unit-tests/probability-helpers.h
        Battle/unit-tests/probability-helpers.cpp
        Battle/unit-tests/test-main.cpp)

add_executable(unit-test-BattleStateMachine
        ${BATTLE_TEST_FILES})

target_link_libraries(unit-test-BattleStateMachine gtest_main Pokemon_Battle_Sim)
add_test(NAME unit-test-BattleStateMachine COMMAND unit-test-BattleStateMachine)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
