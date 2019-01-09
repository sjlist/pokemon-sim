//
// Created by slist on 1/9/19.
//

#ifndef POKEMON_SIM_FIELD_OBJECTS_H
#define POKEMON_SIM_FIELD_OBJECTS_H

#include <string>

enum Field_Objects
{
    STEALTH_ROCKS = 0,
    STICKY_WEB,
    SPIKES,
    TOXIC_SPIKES,
    WEATHER,
    TERRAIN,
    TRICK_ROOM,
    NUM_FIELD_OBJECTS
};

Field_Objects string_to_field_obj(std::string string_obj);

#endif //POKEMON_SIM_FIELD_OBJECTS_H
