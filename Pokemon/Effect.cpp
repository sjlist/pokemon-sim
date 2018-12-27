//
// Created by slist on 12/27/18.
//

#include "Effect.h"
#include <map>
#include <string>

std::map<std::string, MOVE_EFFECTS> string_move_effect_map = {
        {"FLINCH", MOVE_EFFECTS::FLINCH}
};

MOVE_EFFECTS string_to_move_effect(std::string effect_string)
{
    return string_move_effect_map[effect_string];
}