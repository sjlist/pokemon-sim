//
// Created by slist on 1/9/19.
//

#ifndef POKEMON_SIM_FIELDOBJECTS_H
#define POKEMON_SIM_FIELDOBJECTS_H

#include <string>

enum FieldObjects
{
    STEALTH_ROCKS = 0,
    STICKY_WEB,
    SPIKES,
    TOXIC_SPIKES,
    WEATHER,
    TERRAIN,
    TRICK_ROOM,
    CLEAR,
    LEECH_SEED,
    NUM_FIELD_OBJECTS
};

FieldObjects string_to_field_obj(std::string string_obj);

#endif //POKEMON_SIM_FIELDOBJECTS_H
