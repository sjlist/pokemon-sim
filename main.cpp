//
// Created by slist on 12/12/18.
//

#include <iostream>
#include "Type.h"

int main()
{
    PokeTypes pokemon_type [2] = {PokeTypes::FIRE, PokeTypes::FLYING};
    float e = calculate_damage_modifier(pokemon_type, PokeTypes::WATER);
    std::cout << e <<"\n";
    return 1;
}
