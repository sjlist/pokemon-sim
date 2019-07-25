//
// Created by slist on 12/12/18.
//



#include <fileIO/loadJSON.h>
#include <Pokemon/Move.h>
#include <Pokemon/Natures.h>
#include <Pokemon/Pokemon.h>
#include <Pokemon/Stats.h>
#include <Pokemon/Status.h>
#include <Pokemon/Type.h>
#include <Utils/Logging.h>

#include <boost/property_tree/ptree.hpp>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
static int MAX_STAGES = 6;

Pokemon::Pokemon()
{
    Pokemon::active = false;
    Pokemon::alive = false;
    Pokemon::status = STATUS::NO_STATUS;
    Pokemon::substitute_hp = 0;
    Pokemon::current_hp = 0;
    Pokemon::protect_active = false;
    Pokemon::protect_turns = 0;
    for(int stat = STAT::HP; stat < STAT::NUM_STATS; stat++)
        Pokemon::stat_modifiers[stat] = 0;
    for(int i = 0; i < NUM_VOLATILE_STATUS; i++)
        Pokemon::v_status_turns[i] = 0;
    Pokemon::protect_active = false;
    Pokemon::substitute_hp = 0;
}

// STATE CHECKING FUNCTIONS
bool Pokemon::is_active()
{
    return Pokemon::active;
}

int Pokemon::get_stat_mod(STAT stat)
{
    return Pokemon::stat_modifiers[stat];
}

float Pokemon::get_stat(STAT stat)
{
    int mod = Pokemon::get_stat_mod(stat);
    float adjustment = 1;
    float equ_const;

    if(stat == STAT::ACC || stat == STAT::EVA)
        equ_const = 3.0;
    else
        equ_const = 2.0;

    if(stat == STAT::SPE && Pokemon::status == STATUS::PARALYZED)
        adjustment *= 0.5;

    if(mod < 0)
    {
        adjustment *= equ_const / (equ_const - mod);
    }
    else if(mod > 0)
    {
        adjustment *= (equ_const + mod) / equ_const;
    }

    return Pokemon::base_stats[stat] * adjustment;
}

STATUS Pokemon::get_status()
{
    return Pokemon::status;
}

bool Pokemon::is_volatile_status(VOLATILE_STATUS v_status)
{
    return Pokemon::volatile_status & v_status;
}

int Pokemon::get_volatile_status()
{
    return Pokemon::volatile_status;
}


int Pokemon::get_level()
{
    return Pokemon::level;
}

PokeTypes* Pokemon::get_type()
{
    return Pokemon::current_type;
}

string Pokemon::get_species()
{
    return Pokemon::species;
}

int Pokemon::get_v_status_turns(VOLATILE_STATUS_NUMBERS v_status)
{
    return Pokemon::v_status_turns[v_status];
}

int Pokemon::get_protect_turns()
{
    return Pokemon::protect_turns;
}

bool Pokemon::is_alive()
{
    return Pokemon::alive;
}

float Pokemon::get_current_hp()
{
    return Pokemon::current_hp;
}

bool Pokemon::is_grounded()
{
    if(Pokemon::current_type[0] == PokeTypes::FLYING || Pokemon::current_type[1] == PokeTypes::FLYING)
        return false;

    return true;
}

bool Pokemon::is_protected()
{
    return Pokemon::protect_active;
}

// STATE CHANGE FUNCTIONS

bool Pokemon::use_move(int move_number)
{
    return Pokemon::moves[move_number].use();
}

bool Pokemon::setup_substitute()
{
    if(Pokemon::current_hp <= (Pokemon::base_stats[STAT::HP] / 4) || Pokemon::substitute_hp != 0)
        return false;

    Pokemon::current_hp -= (Pokemon::base_stats[STAT::HP] / 4);
    Pokemon::substitute_hp = (Pokemon::base_stats[STAT::HP] / 4);
    return true;
}

bool Pokemon::deal_damage(float damage, bool ignore_sub)
{
    if(Pokemon::substitute_hp > 0 && !ignore_sub)
    {
        DEBUG_MSG("Substitue took damage for " << Pokemon::species << endl);
        Pokemon::substitute_hp -= damage;
        if(Pokemon::substitute_hp <= 0)
        {
            Pokemon::substitute_hp = 0;
            DEBUG_MSG("Substitue faded\n");
        }
        return true;
    }
    DEBUG_MSG("Dealt " << round((float)damage/Pokemon::base_stats[STAT::HP]*100*10)/10 << "% damage to " << Pokemon::species << "\n");
    Pokemon::current_hp -=  damage;

    if(Pokemon::current_hp <= 0)
    {
        Pokemon::current_hp = 0;
        return false;
    }
    return true;
}

void Pokemon::heal_damage(int damage)
{
    DEBUG_MSG("Healed " << round((float)damage/Pokemon::base_stats[STAT::HP]*100*10)/10 << "% hitpoints to " << Pokemon::species << "\n");

    Pokemon::current_hp = Pokemon::current_hp + damage;

    if(Pokemon::current_hp >= Pokemon::base_stats[STAT::HP])
    {
        Pokemon::current_hp = Pokemon::base_stats[STAT::HP];
    }
}

bool Pokemon::set_status(STATUS new_status)
{
    if(Pokemon::status != STATUS::NO_STATUS && new_status != STATUS::NO_STATUS)
    {
        DEBUG_MSG(Pokemon::species << " is already " << status_to_string(Pokemon::status) << " and cannot be " << status_to_string(new_status) << "\n");
        return false;
    }

    switch(new_status)
    {
        case STATUS::ASLEEP:
            Pokemon::status_turns = 0;
            break;
        case STATUS::BURNED:
            if(Pokemon::type[0] == PokeTypes::FIRE || Pokemon::type[1] == PokeTypes::FIRE)
            {
                DEBUG_MSG(Pokemon::species << " cannot be " << status_to_string(new_status) << endl);
                return false;
            }
            break;
        case STATUS::FROZEN:
            if(Pokemon::type[0] == PokeTypes::ICE || Pokemon::type[1] == PokeTypes::ICE)
            {
                DEBUG_MSG(Pokemon::species << " cannot be " << status_to_string(new_status) << endl);
                return false;
            }
            break;
        case STATUS::PARALYZED:
            if(Pokemon::type[0] == PokeTypes::ELECTRIC || Pokemon::type[1] == PokeTypes::ELECTRIC)
            {
                DEBUG_MSG(Pokemon::species << " cannot be " << status_to_string(new_status) << endl);
                return false;
            }
            break;
        case STATUS::BADLY_POISONED:
            Pokemon::status_turns = 0;
        case STATUS::POISONED:
            if(Pokemon::type[0] == PokeTypes::STEEL || Pokemon::type[1] == PokeTypes::STEEL || Pokemon::type[0] == PokeTypes::POISON || Pokemon::type[1] == PokeTypes::POISON)
            {
                DEBUG_MSG(Pokemon::species << " cannot be " << status_to_string(new_status) << endl);
                return false;
            }
            break;
        case STATUS::NO_STATUS:
            break;
        default:
            ERR_MSG("Unhandled status " << new_status << "\n");
    }

    Pokemon::status = new_status;
    return true;
}

bool Pokemon::set_volatile_status(VOLATILE_STATUS v_status)
{
    if(!Pokemon::is_volatile_status(v_status))
    {
        Pokemon::volatile_status |= v_status;
        return true;
    }
    else
    {
        DEBUG_MSG(Pokemon::species << " is already " << v_status_to_string(v_status) << "\n");
        return false;
    }
}

void Pokemon::set_active(bool state)
{
    Pokemon::active = state;
}

void Pokemon::faint_poke()
{
    Pokemon::alive = false;
}

void Pokemon::increment_v_status_turns(VOLATILE_STATUS_NUMBERS v_status)
{
    Pokemon::v_status_turns[v_status]++;
}

void Pokemon::increment_protect_turns()
{
    Pokemon::protect_turns++;
    if(Pokemon::protect_turns > 6)
        Pokemon::protect_turns = 6;
}


void Pokemon::remove_type(int type_num)
{
    Pokemon::current_type[type_num] = PokeTypes::NO_TYPE;
}

void Pokemon::protect_poke()
{
    Pokemon::protect_active = true;
}

//CLEARING FUNCTIONS

void Pokemon::clear_stat_mods()
{
    for(int i = 0; i < STAT::NUM_STATS; i++)
        Pokemon::stat_modifiers[i]  = 0;
}

void Pokemon::clear_volatile_status(VOLATILE_STATUS v_status)
{
    Pokemon::volatile_status &= ~(v_status);
}

void Pokemon::clear_volatile_status_mask(int v_status_mask)
{
    Pokemon::volatile_status &= ~(v_status_mask);
}

void Pokemon::clear_volatile_statuses()
{
    Pokemon::volatile_status = 0;
    for(int i = 0; i < NUM_VOLATILE_STATUS; i++)
        Pokemon::v_status_turns[i] = 0;
}

void Pokemon::clear_v_status_turns(VOLATILE_STATUS_NUMBERS v_status)
{
    Pokemon::v_status_turns[v_status] = 0;
}

void Pokemon::clear_protect_turns()
{
    Pokemon::protect_turns = 0;
}

void Pokemon::reset_types()
{
    Pokemon::current_type[0] = Pokemon::type[0];
    Pokemon::current_type[1] = Pokemon::type[1];
}
//

void Pokemon::stat_change(STAT stat, int stages)
{
    if(stages < 0)
        DEBUG_MSG(Pokemon::species << "'s " << stat_to_string(stat) << " dropped by " << to_string(abs(stages)) << " stage(s)\n");

    if(stages > 0)
        DEBUG_MSG(Pokemon::species << "'s " << stat_to_string(stat) << " rose by " << to_string(stages) << " stage(s)\n");

    Pokemon::stat_modifiers[stat] = Pokemon::stat_modifiers[stat] + stages;

    if(Pokemon::stat_modifiers[stat] < (-1 * MAX_STAGES))
    {
        DEBUG_MSG(Pokemon::species << "'s " << stat_to_string(stat) << " can't go lower\n");
        Pokemon::stat_modifiers[stat] = -1 * MAX_STAGES;
    }

    if(Pokemon::stat_modifiers[stat] > MAX_STAGES)
    {
        DEBUG_MSG(Pokemon::species << "'s " << stat_to_string(stat) << " can't go higher\n");
        Pokemon::stat_modifiers[stat] = MAX_STAGES;
    }
}

void Pokemon::reset_protect()
{
    Pokemon::protect_active = false;
}
//

// LOADING POKEMON FUNCTIONS

void Pokemon::reset()
{
    Pokemon::active = false;
    for(int stat = STAT::HP; stat < STAT::NUM_STATS; stat++)
    {
        Pokemon::stat_modifiers[stat] = 0;
    }
    Pokemon::alive = true;
    Pokemon::current_hp = Pokemon::base_stats[STAT::HP];
    Pokemon::clear_volatile_statuses();
    Pokemon::status = STATUS::NO_STATUS;
    for(int i = 0; i < 4; i++)
    {
        if(Pokemon::moves[i].get_name().empty())
            break;
        else
            Pokemon::moves[i].reset();
    }
    for(int i = 0; i < NUM_VOLATILE_STATUS; i++)
        Pokemon::v_status_turns[i] = 0;

    Pokemon::reset_protect();
    Pokemon::to_be_swapped = false;
}

void Pokemon::load_pokemon(boost::property_tree::ptree poke_ptree)
{
    int evs [NUM_STATS], ivs [NUM_STATS];
    auto ev_ptr = evs;
    auto iv_ptr = ivs;

    Pokemon::load_species(poke_ptree.get<string>("species"));
    Pokemon::level = poke_ptree.get<int>("level", 0);

    boost::property_tree::ptree evs_ptree = poke_ptree.get_child("evs");
    boost::property_tree::ptree ivs_ptree = poke_ptree.get_child("ivs");

    for(int i = 0; i < STAT::NUM_STATS; i++)
    {
        evs[i] = evs_ptree.get(to_string(i), 0);
        ivs[i] = ivs_ptree.get(to_string(i), 0);
    }

    Pokemon::set_stats(iv_ptr, ev_ptr, level, string_to_nature(poke_ptree.get<string>("nature")));

    boost::property_tree::ptree moves_ptree = poke_ptree.get_child("moves");


    string move_name;
    for(int i = 0; i < 4; i++)
    {
        move_name = moves_ptree.get<string>("MOVE" + to_string(i));
        if(!move_name.empty())
        {
            Pokemon::moves[i].load_move(move_name);
        }
    }

    Pokemon::alive = true;
    Pokemon::status = STATUS::NO_STATUS;
    Pokemon::status_turns = 0;
    Pokemon::volatile_status = 0;
    Pokemon::to_be_swapped = false;
}

void Pokemon::load_species(string species_name)
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree root_child;

    try
    {
        root = load_json_file("species/" + species_name + ".json");
    }
    catch(...)
    {
        ERR_MSG("ERROR: FAILED TO LOAD SPECIES " << species_name << "\n");
    }

    Pokemon::base_stats[STAT::HP]  = root.get<int>("HP", 0);
    Pokemon::base_stats[STAT::ATK] = root.get<int>("ATK", 0);
    Pokemon::base_stats[STAT::DEF] = root.get<int>("DEF", 0);
    Pokemon::base_stats[STAT::SPA] = root.get<int>("SPA", 0);
    Pokemon::base_stats[STAT::SPD] = root.get<int>("SPD", 0);
    Pokemon::base_stats[STAT::SPE] = root.get<int>("SPE", 0);

    Pokemon::species = species_name;

    root_child = root.get_child("TYPES");
    Pokemon::type[0] = string_to_type(root_child.get<string>("TYPE0"));
    Pokemon::type[1] = string_to_type(root_child.get<string>("TYPE1"));

    Pokemon::current_type[0] = Pokemon::type[0];
    Pokemon::current_type[1] = Pokemon::type[1];
}

float Pokemon::calculate_hp(int level, int base_hp, int ev_hp, int iv_hp)
{
    return floor(((2 * base_hp + iv_hp + floor((float)(ev_hp / 4))) * level) / 100) + level + 10;
}

float Pokemon::calculate_stat_single(int level, int base, int ev, int iv, float nature_mod)
{
    return (int)((floor(floor((float)((float)ev / 4) + iv + 2 * base) / 100 * level) + 5) * nature_mod);
}

void Pokemon::set_stats(int* ivs, int* evs, int level, Natures nature)
{
    float nature_mod;
    for(int i = STAT::HP; i < STAT::NUM_STATS; i++)
    {
        switch(i)
        {
            case STAT::HP:
                Pokemon::base_stats[i] = calculate_hp(level, Pokemon::base_stats[i], evs[i], ivs[i]);
                Pokemon::current_hp = Pokemon::base_stats[i];
                break;
            case STAT::ATK:
            case STAT::DEF:
            case STAT::SPA:
            case STAT::SPD:
            case STAT::SPE:
                nature_mod = get_nature_mod(nature, (STAT)i);
                Pokemon::base_stats[i] = calculate_stat_single(level, Pokemon::base_stats[i], evs[i], ivs[i], nature_mod);
                break;
            case STAT::ACC:
            case STAT::EVA:
                Pokemon::base_stats[i] = 1;
                break;
            default:
                ERR_MSG("Unhandled stat " << i << endl);
        }
    }
}
//


void Pokemon::print_pokemon(bool detailed)
{
    if(Pokemon::species.empty())
        return;

    DEBUG_MSG(Pokemon::species << "\n");
    if(detailed)
    {
        DEBUG_MSG("HP:  "  << Pokemon::base_stats[STAT::HP] << " Modifier: " << Pokemon::stat_modifiers[STAT::HP]  << "\n");
        DEBUG_MSG("ATK: " << Pokemon::get_stat(STAT::ATK) << " Modifier: " << Pokemon::stat_modifiers[STAT::ATK] << "\n");
        DEBUG_MSG("DEF: " << Pokemon::get_stat(STAT::DEF) << " Modifier: " << Pokemon::stat_modifiers[STAT::DEF] << "\n");
        DEBUG_MSG("SPA: " << Pokemon::get_stat(STAT::SPA) << " Modifier: " << Pokemon::stat_modifiers[STAT::SPA] << "\n");
        DEBUG_MSG("SPD: " << Pokemon::get_stat(STAT::SPD) << " Modifier: " << Pokemon::stat_modifiers[STAT::SPD] << "\n");
        DEBUG_MSG("SPE: " << Pokemon::get_stat(STAT::SPE) << " Modifier: " << Pokemon::stat_modifiers[STAT::SPE] << "\n");
        DEBUG_MSG("Level: " << Pokemon::level << "\n");
        DEBUG_MSG("ACTIVE: " << Pokemon::active << "\n");
        DEBUG_MSG("ALIVE: " << Pokemon::alive << "\n");
        Pokemon::moves[0].print_move();
        Pokemon::moves[1].print_move();
    }
    DEBUG_MSG("Current HP: " << round(Pokemon::current_hp / Pokemon::base_stats[STAT::HP] * 100 * 10) / 10 << "%\n");
    DEBUG_MSG("STATUS: " << status_to_string(Pokemon::status) << "\n");
}


// UNIT TEST HELPERS
void Pokemon::create_test_pokemon(PokeTypes t1, PokeTypes t2, Natures n, float pcent_hp, string species)
{
    int evs [STAT::NUM_STATS], ivs [STAT::NUM_STATS];
    auto ev_ptr = evs;
    auto iv_ptr = ivs;

    Pokemon::level = 50;

    for(int i = 0; i < STAT::NUM_STATS; i++)
    {
        evs[i] = 0;
        ivs[i] = 0;
    }

    Pokemon::alive = true;
    Pokemon::active = true;
    Pokemon::status = STATUS::NO_STATUS;
    Pokemon::status_turns = 0;
    Pokemon::volatile_status = 0;
    Pokemon::to_be_swapped = false;

    Pokemon::load_species(species);

    Pokemon::set_stats(iv_ptr, ev_ptr, level, n);

    Pokemon::name = "TESTEMON";

    Pokemon::type[0] = t1;
    Pokemon::type[1] = t2;

    Pokemon::current_type[0] = Pokemon::type[0];
    Pokemon::current_type[1] = Pokemon::type[1];

    Pokemon::current_hp = Pokemon::base_stats[STAT::HP] * pcent_hp / 100;
    Pokemon::protect_active = false;
    Pokemon::protect_turns = 0;
}