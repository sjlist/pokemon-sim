//
// Created by slist on 12/19/18.
//

#include "BattleActor.h"
#include <iostream>
#include <string>

BattleActor::BattleActor()
{

}

int BattleActor::choose_action()
{
    std::string action;
    std::cout << "Choose an action:";
    std::cin >> action;
    std::cout << action << "\n";
}

int BattleActor::choose_move() {}
int BattleActor::choose_pokemon() {}
int BattleActor::choose_target() {}