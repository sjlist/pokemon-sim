//
// Created by slist on 12/12/18.
//

#include <iostream>
#include "Type.h"
#include "Pokemon.h"
#include "Battle.h"
#include "Move.h"

int main()
{
    Battle battle;
    battle.load_battle();
    //Move flame;
    //flame.load_move("Flamethrower");
    //flame.print_move();

    battle.init();
    // Pokemon gengar;
    // Pokemon garchomp;
    // int base [6] = {130, 95, 80, 85, 102, 108};
    // int iv [6] = {31, 31, 31, 31 ,31, 31};
    // int ev [6] = {252, 0, 0, 0, 252, 8};
    // garchomp.set_stats(base , iv, ev, 50, Natures::ADAMANT);
    //
    // std::cout << "ATK " << garchomp.get_stat(STAT::ATK) << "\n";
    // std::cout << "DEF " << garchomp.get_stat(STAT::DEF) << "\n";
    // std::cout << "POW " << garchomp.moves[0].get_power() << "\n";
    //
    // float damage_mod = calculate_damage_modifier(garchomp.get_type(), garchomp.moves[0].get_type());
    // int base_damage = ((((2 * garchomp.get_level() / 5) + 2)* garchomp.moves[0].get_power() * garchomp.get_stat(STAT::ATK) / (garchomp.get_stat(STAT::DEF) * 50)) + 2) * damage_mod;
    // std::cout << base_damage << "\n";
    return 1;
}
