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
    int num_runs = 0;
    BattleStateMachine BSM = *new BattleStateMachine();
    BSM.run();
    while(num_runs >= 10000)
    {
        num_runs++;
        std::cout << "RUN: " << num_runs << std::endl;
        BSM.reset();
        BSM.run(BattleState::BATTLE_START);
    }
}
