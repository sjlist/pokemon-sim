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

private:
    std::vector<std::string> select_teams();
    void init();
    void load_teams(std::vector<std::string> team_names);

    Pokemon p1_party [6];
    Pokemon p2_party [6];
};


#endif //POKEMON_SIM_BATTLE_H
