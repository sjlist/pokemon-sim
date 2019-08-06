//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLEACTOR_H
#define POKEMON_SIM_BATTLEACTOR_H

#include <Battle/Battle.h>
#include <Battle/Field.h>
#include <Battle/Players.h>
#include <Battle/BattleMessage.h>
using namespace std;

class BattleActor
{
public:
    BattleActor();
    BattleActor(long seed);

    float attack_swap_ratio [2] = {0.8, 0.8};

    int choose_pokemon(Party* party);
    BattleMessage choose_action(FIELD_POSITION atk_pos, Party* player_party, Field* field, bool player_can_mega, Actions action=Actions::CHOOSE_ACTION);
    void update_generator(long seed);
protected:
    int choose_position(int num_positions);
    bool roll_chance(float chance);

private:
    mt19937 generator;
    uniform_real_distribution<float> pcent_chance;
    uniform_int_distribution<int> poke_choice;
    Targets actor_targeting;

    FIELD_POSITION choose_target(FIELD_POSITION atk_pos, int num_moves, TARGETS targets, Field* field);
    int choose_move(Pokemon* poke);
};
#endif //POKEMON_SIM_BATTLEACTOR_H
