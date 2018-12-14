//
// Created by slist on 12/14/18.
//

#include "Field.h"

Field::Field()
{
    Field::spikes = false;
    Field::toxic_spikes = false;
    Field::stealth_rocks = false;
    Field::trick_room = false;
    Field::terrain = Terrain::NO_TERRAIN;
    Field::weather_state = Weather::CLEAR_SKIES;
}