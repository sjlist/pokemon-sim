//
// Created by slist on 12/14/18.
//

#include "Players.h"
#include "Field.h"
#include <iostream>

Field::Field()
{
    Field::spikes = 0;
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
        Field::active_pokes[player].set_active(true);
        return true;
    }
    else
        return false;
}

bool Field::active_open(Players player)
{
    return !Field::active_pokes[player].is_active();
}

void Field::print_field(bool detailed)
{
    std::cout << "ACTIVE POKEMON: " << "\nPLAYER ONE\n";
    Field::active_pokes[Players::PLAYER_ONE].print_pokemon(detailed);
    std::cout << "\nPLAYER TWO\n";
    Field::active_pokes[Players::PLAYER_TWO].print_pokemon(detailed);
    if(detailed)
    {
        std::cout << "FIELD STATE:\n";
        if(Field::spikes > 0)
            std::cout << Field::spikes << " layers of spikes\n";
        if(Field::toxic_spikes)
            std::cout << "Toxic spikes\n";
        if(Field::stealth_rocks)
            std::cout << "Stealth rocks\n";
        if(Field::trick_room)
            std::cout << "Trick room active\n";
        if(Field::terrain != Terrain::NO_TERRAIN)
            std::cout << Field::terrain << " terrain is active\n";
        if(Field::weather_state != Weather::CLEAR_SKIES)
            std::cout << Field::terrain << " weather is active\n";
    }
}
