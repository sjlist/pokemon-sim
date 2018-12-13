//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_POKEMON_H
#define POKEMON_SIM_POKEMON_H

#include "Move.h"
#include "Natures.h"

class Pokemon {
public:
    Pokemon(std::string species_name);
    PokeTypes* get_type();
    int get_level();
    int get_stat(STAT stat);

    Move moves[4];

    void set_stats(int* base_stats, int* ivs, int* evs, int level, Natures nature);
private:

    int level;

    PokeTypes type[2];

    int current_hp;
    int base_stats [6];

    int stat_modifiers [5];

    std::string name;
    int calculate_hp(int level, int base_hp, int ev_hp, int iv_hp);
    int calculate_stat_single(int level, int base, int ev, int iv, float nature_mod);

};


#endif //POKEMON_SIM_POKEMON_H
