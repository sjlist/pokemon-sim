//
// Created by slist on 12/12/18.
//

#include "Move.h"
#include <string>

Move::Move()
{
    name = "Red";
    max_pp = 10;
    current_pp = max_pp;
    power = 100;
    acc = 100;
    type = PokeTypes::GROUND;
}

std::string Move::get_name()
{
    return Move::name;
}

int Move::get_pp()
{
    return Move::current_pp;
}

int Move::get_power()
{
    return Move::power;
}

int Move::get_acc()
{
    return Move::acc;
}

PokeTypes Move::get_type()
{
    return Move::type;
}