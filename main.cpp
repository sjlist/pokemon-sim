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
    battle.init();

    battle.attack(Players::PLAYER_ONE, 0);
    battle.attack(Players::PLAYER_ONE, 0);
    battle.attack(Players::PLAYER_ONE, 0);
    battle.attack(Players::PLAYER_ONE, 0);
    battle.attack(Players::PLAYER_ONE, 0);
    battle.attack(Players::PLAYER_ONE, 0);
    battle.active_field.print_field();


    return 1;
}
