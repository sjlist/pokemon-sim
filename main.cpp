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
    int wins [3] = {0, 0, 0}, max_runs = 10000;
    int num_runs = 1, winner;
    BattleStateMachine BSM = *new BattleStateMachine(80905532);
    winner = BSM.run() + 1;
    wins[winner] += 1;
    long seed;
    while(num_runs < max_runs)
    {
        num_runs++;
        std::cout << "RUN: " << num_runs << std::endl;
        BSM.reset();
        seed = BSM.get_seed();
        winner = BSM.run(BattleState::BATTLE_START) + 1;
        wins[winner] += 1;
    }

    std::cout << "Player 1 won " << wins[2] / (float)max_runs * 100 << "% of the time\n";
    std::cout << "Player 2 won " << wins[0] / (float)max_runs * 100 << "% of the time\n";
    std::cout << "Players tied " << wins[1] / (float)max_runs * 100 << "% of the time\n";
}
