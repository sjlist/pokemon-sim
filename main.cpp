//
// Created by slist on 12/12/18.
//

#include <iostream>
#include "Pokemon/Type.h"
#include "Pokemon/Pokemon.h"
#include "Battle/Battle.h"
#include "Battle/BattleStateMachine.h"
#include "Battle/Targeting.h"
#include "Pokemon/Move.h"

int main()
{

    BattleStateMachine BSM = *new BattleStateMachine();
    BSM.run();
//    Targets t;
//
//    t.get_valid_targets(TARGETS::ADJACENT_ALL, FIELD_POSITION::PLAYER_2_0);
//    std::cout << "ADJACENT_ALL\n";
//    for(int i = 0; i < t.num_valid_targets; i++)
//        std::cout << t.valid_targets[i] << "\n";
//
//    t.get_valid_targets(TARGETS::ALL_FRIENDLY, FIELD_POSITION::PLAYER_2_0);
//    std::cout << "ALL_FRIENDLY\n";
//    for(int i = 0; i < t.num_valid_targets; i++)
//        std::cout << t.valid_targets[i] << "\n";
//
//    t.get_valid_targets(TARGETS::SELF, FIELD_POSITION::PLAYER_2_0);
//    std::cout << "SELF\n";
//    for(int i = 0; i < t.num_valid_targets; i++)
//        std::cout << t.valid_targets[i] << "\n";
//
//    t.get_valid_targets(TARGETS::ALL_ALL, FIELD_POSITION::PLAYER_2_0);
//    std::cout << "ALL_ALL\n";
//    for(int i = 0; i < t.num_valid_targets; i++)
//        std::cout << t.valid_targets[i] << "\n";
}
