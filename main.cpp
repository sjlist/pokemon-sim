//
// Created by slist on 12/12/18.
//

#include <Battle/BattleStateMachine.h>
#include <iostream>


int main()
{
    int wins [3] = {0, 0, 0}, max_runs = 10000;
    int num_runs = 1, winner, max_turns = 0, tot_turns = 0;
    std::cout << "RUN: " << num_runs << std::endl;
    BattleStateMachine BSM = *new BattleStateMachine(RAND_SEED);
    winner = BSM.run() + 1;
    wins[winner] += 1;
    if(BSM.get_turn_count() > max_turns)
        max_turns = BSM.get_turn_count();
    tot_turns += BSM.get_turn_count();
    long seed;
    while(num_runs < max_runs)
    {
        num_runs++;
        std::cout << "RUN: " << num_runs << std::endl;
        BSM.reset();
        seed = BSM.get_seed();
        winner = BSM.run(BattleState::BATTLE_START) + 1;
        wins[winner] += 1;
        if(BSM.get_turn_count() > max_turns)
            max_turns = BSM.get_turn_count();
        tot_turns += BSM.get_turn_count();
    }

    std::cout << std::endl;
    std::cout << "Player 1 won " << wins[2] / (float)max_runs * 100 << "% of the time\n";
    std::cout << "Player 2 won " << wins[0] / (float)max_runs * 100 << "% of the time\n";
    std::cout << "Max turn count " << max_turns << "\n";
    std::cout << "Average turn count " << tot_turns / (float)max_runs << "\n";
}
