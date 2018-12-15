//
// Created by slist on 12/14/18.
//

#include "Players.h"
#include "Field.h"
#include <iostream>

Field::Field()
{
    Field::spikes = false;
    Field::toxic_spikes = false;
    Field::stealth_rocks = false;
    Field::trick_room = false;
    Field::terrain = Terrain::NO_TERRAIN;
    Field::weather_state = Weather::CLEAR_SKIES;
}

bool Field::send_out(Players player, Pokemon poke)
{
    if(Field::active_open(player))
    {
        Field::active_pokes[player] = poke;
        return true;
    }
    else
        return false;
}

bool Field::active_open(Players player)
{
    if(Field::active_pokes[player].is_active())
        return false;
    else
        return true;
}

void Field::print_field()
{
    Field::active_pokes[Players::PLAYER_ONE].print_pokemon();
}
