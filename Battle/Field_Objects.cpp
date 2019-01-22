//
// Created by slist on 1/9/19.
//

#include <Battle/Field_Objects.h>
#include <map>
#include <string>

std::map<std::string, Field_Objects> string_v_field_obj_map = {
        { "STEALTH_ROCKS", Field_Objects::STEALTH_ROCKS },
        { "STICKY_WEB", Field_Objects::STICKY_WEB },
        { "SPIKES", Field_Objects::SPIKES },
        { "TOXIC_SPIKES", Field_Objects::TOXIC_SPIKES },
        { "WEATHER", Field_Objects::WEATHER },
        { "TERRAIN", Field_Objects::TERRAIN },
        { "TRICK_ROOM", Field_Objects::TRICK_ROOM },
        { "LEECH_SEED", Field_Objects::LEECH_SEED},
        { "CLEAR", Field_Objects::CLEAR },
        { "NUM_FIELD_OBJECTS", Field_Objects::NUM_FIELD_OBJECTS }
};

Field_Objects string_to_field_obj(std::string string_obj)
{
    return string_v_field_obj_map[string_obj];
}