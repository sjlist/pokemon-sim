//
// Created by slist on 12/27/18.
//

#include <Pokemon/Effect.h>
#include <Pokemon/Type.h>
#include <Utils/Logging.h>

#include <boost/property_tree/ptree.hpp>
#include <map>
#include <string>
using namespace std;

map<string, MOVE_EFFECTS> string_move_effect_map = {
        {"SWAP", MOVE_EFFECTS::SWAP},
        {"STATUS", MOVE_EFFECTS::NON_VOLATILE_STATUS_EFFECT},
        {"VOLATILE_STATUS", MOVE_EFFECTS::VOLATILE_STATUS_EFFECT},
        {"STAT_CHANGE", MOVE_EFFECTS::STAT_CHANGE},
        {"FIELD_CHANGE", MOVE_EFFECTS::FIELD_CHANGE},
        {"RECOIL", MOVE_EFFECTS::RECOIL},
        {"HEAL", MOVE_EFFECTS::HEAL},
        {"REMOVE_TYPE", MOVE_EFFECTS::REMOVE_TYPE},
        {"FLAT_DAMAGE", MOVE_EFFECTS::FLAT_DAMAGE},
        {"PROTECT", MOVE_EFFECTS::PROTECT},
        {"SUBSTITUTE", MOVE_EFFECTS::SUBSTITUTE},
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

bool Effect::does_target_self()
{
    return Effect::target_self;
}

STATUS Effect::get_effect_status_type()
{
    return Effect::status_effect;
}

VOLATILE_STATUS Effect::get_volatile_status_effect()
{
    return Effect::v_status_effect;
}

float Effect::get_effect_chance()
{
    return Effect::effect_chance;
}

STAT Effect::get_stat_changed()
{
    return Effect::stat_changed;
}

int Effect::get_stages_changed()
{
    return Effect::stages_changes;
}

FieldObjects Effect::get_field_obj_changed()
{
    return Effect::field_obj;
}

bool Effect::get_use_damage()
{
    return Effect::use_damage;
}

float Effect::get_percent_recoil()
{
    return Effect::percent_recoil;
}

float Effect::get_heal_percent()
{
    return Effect::heal_percent;
}

PokeTypes Effect::get_type_removed()
{
    return Effect::type_removed;
}

bool Effect::use_flat_level()
{
    return Effect::flat_damage_level;
}

int Effect::get_flat_damage()
{
    return Effect::flat_damage;
}

// Load effect
void Effect::load_effect(boost::property_tree::ptree effect_tree)
{
    Effect::effect_type = string_to_move_effect(effect_tree.get<string>("effect"));

    if(effect_tree.count("target"))
        Effect::target_self = (effect_tree.get<string>("target") == "SELF");
    else
        Effect::target_self = false;


    if(effect_tree.count("chance"))
        Effect::effect_chance = (float)effect_tree.get<int>("chance") / 100;
    else
        Effect::effect_chance = 100;

    switch(Effect::effect_type)
    {
        case MOVE_EFFECTS::VOLATILE_STATUS_EFFECT:
            Effect::v_status_effect = string_to_v_status(effect_tree.get<string>("status"));
            break;
        case MOVE_EFFECTS::NON_VOLATILE_STATUS_EFFECT:
            Effect::status_effect = string_to_status(effect_tree.get<string>("status"));
            break;
        case MOVE_EFFECTS::STAT_CHANGE:
            Effect::stat_changed = string_to_stat(effect_tree.get<string>("stat"));
            Effect::stages_changes = effect_tree.get<int>("stages");
            break;
        case MOVE_EFFECTS::FIELD_CHANGE:
            Effect::field_obj = string_to_field_obj(effect_tree.get<string>("field_object"));
            break;
        case MOVE_EFFECTS::RECOIL:
            Effect::use_damage = (effect_tree.get<string>("recoil_type") == "DAMAGE");
            Effect::percent_recoil = effect_tree.get<int>("percent") / 100.0;
            break;
        case MOVE_EFFECTS::HEAL:
            Effect::heal_percent = effect_tree.get<int>("heal_percent") / 100.0;
            break;
        case MOVE_EFFECTS::REMOVE_TYPE:
            Effect::type_removed = string_to_type(effect_tree.get<string>("type_removed"));
            break;
        case MOVE_EFFECTS::FLAT_DAMAGE:
            if(effect_tree.count("flat_damage"))
            {
                Effect::flat_damage_level = false;
                Effect::flat_damage = effect_tree.get<int>("flat_damage");
            }
            else
            {
                Effect::flat_damage_level = true;
            }
            break;
        case MOVE_EFFECTS::SWAP:
        case MOVE_EFFECTS::PROTECT:
        case MOVE_EFFECTS::SUBSTITUTE:
            break;
        default:
            ERR_MSG("Unhandled Effect Type: " << Effect::effect_type << endl);
    }

}



//NON CLASS METHODS
MOVE_EFFECTS string_to_move_effect(string effect_string)
{
    return string_move_effect_map[effect_string];
}