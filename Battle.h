//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_BATTLE_H
#define POKEMON_SIM_BATTLE_H

#include "Pokemon.h"
#include "Field.h"
#include <vector>

class Battle {
public:
    Battle();
    Field active_field;
    void load_battle();
    void init();

    void attack(Players player, int move, FIELD_POSITION target);
private:
    std::vector<std::string> select_teams();
    void load_teams(std::vector<std::string> team_names);
    bool roll_crit(float crit_chance);
    bool roll_acc(float acc);
    void send_out(Players player, int poke_position);

    Pokemon p1_party [6];
    Pokemon p2_party [6];
};


#endif //POKEMON_SIM_BATTLE_H
