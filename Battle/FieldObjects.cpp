//
// Created by slist on 1/9/19.
//

#include <Battle/FieldObjects.h>
#include <map>
#include <string>

map<string, FieldObjects> string_v_field_obj_map = {
        { "STEALTH_ROCKS", FieldObjects::STEALTH_ROCKS },
        { "STICKY_WEB", FieldObjects::STICKY_WEB },
        { "SPIKES", FieldObjects::SPIKES },
        { "TOXIC_SPIKES", FieldObjects::TOXIC_SPIKES },
        { "WEATHER", FieldObjects::WEATHER },
        { "TERRAIN", FieldObjects::TERRAIN },
        { "TRICK_ROOM", FieldObjects::TRICK_ROOM },
        { "LEECH_SEED", FieldObjects::LEECH_SEED},
        { "CLEAR", FieldObjects::CLEAR },
        { "NUM_FIELD_OBJECTS", FieldObjects::NUM_FIELD_OBJECTS }
};

FieldObjects string_to_field_obj(string string_obj)
{
    return string_v_field_obj_map[string_obj];
}