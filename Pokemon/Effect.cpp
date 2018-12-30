//
// Created by slist on 12/27/18.
//

#include "Effect.h"

#include <boost/property_tree/ptree.hpp>
#include <map>
#include <string>

std::map<std::string, MOVE_EFFECTS> string_move_effect_map = {
        {"FLINCH", MOVE_EFFECTS::FLINCH},
        {"SWAP", MOVE_EFFECTS::SWAP},
        {"STATUS", MOVE_EFFECTS::STATUS_EFFECT},
        {"NONE", MOVE_EFFECTS::NO_MOVE_EFFECT}
};

Effect::Effect()
{
    Effect::effect_chance = 0;
    Effect::status_effect = STATUS::NO_STATUS;
    Effect::effect_type = MOVE_EFFECTS::NO_MOVE_EFFECT;
};


// Accessing Effects
MOVE_EFFECTS Effect::get_effect()
{
    return Effect::effect_type;
}

STATUS Effect::get_effect_status_type()
{
    return Effect::status_effect;
}

float Effect::get_effect_chance()
{
    return Effect::effect_chance;
}

// Load effect
void Effect::load_effect(boost::property_tree::ptree effect_tree)
{
    Effect::effect_type = string_to_move_effect(effect_tree.get<std::string>("effect"));
    if(effect_tree.count("status"))
        Effect::status_effect = string_to_status(effect_tree.get<std::string>("status"));
    Effect::effect_chance = (float)effect_tree.get<int>("chance") / 100;
}



//NON CLASS METHODS
MOVE_EFFECTS string_to_move_effect(std::string effect_string)
{
    return string_move_effect_map[effect_string];
}