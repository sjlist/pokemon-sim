//
// Created by slist on 1/8/19.
//

#ifndef POKEMON_SIM_PARTY_H
#define POKEMON_SIM_PARTY_H

#include "Pokemon/Pokemon.h"

class Party {
public:
    Party();
    Pokemon party_pokes [6];
    void print_party(bool detailed);
    void reset();
};

#endif //POKEMON_SIM_PARTY_H
