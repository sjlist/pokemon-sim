//
// Created by slist on 12/14/18.
//

#include <Battle/Players.h>
#include <Battle/Field.h>
#include <Utils/Logging.h>

Field::Field()
{
    Field::spikes[Players::PLAYER_ONE] = 0;
    Field::spikes[Players::PLAYER_TWO] = 0;
    Field::toxic_spikes[Players::PLAYER_ONE] = 0;
    Field::toxic_spikes[Players::PLAYER_TWO] = 0;
    Field::stealth_rocks[Players::PLAYER_ONE] = false;
    Field::stealth_rocks[Players::PLAYER_TWO] = false;
    Field::sticky_web[Players::PLAYER_ONE] = false;
    Field::sticky_web[Players::PLAYER_TWO] = false;
    Field::trick_room = false;
    Field::terrain = Terrain::NO_TERRAIN;
    Field::weather_state = Weather::CLEAR_SKIES;

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        Field::leech_seed_positions[i] = FIELD_POSITION::NO_POSITION;
        Field::active_pokes[i] = nullptr;
    }
}

void Field::modify_field_obj(Field_Objects obj, FIELD_POSITION def_pos, FIELD_POSITION atk_pos)
{
    switch(obj)
    {
        case Field_Objects::STEALTH_ROCKS:
            DEBUG_MSG("Rocks are scattered all around P" << get_player_from_position(def_pos) + 1 << "'s field\n");
            Field::stealth_rocks[get_player_from_position(def_pos)] = true;
            break;
        case Field_Objects::STICKY_WEB:
            DEBUG_MSG("Sticky web is scattered all around P" << get_player_from_position(def_pos) + 1 << "'s field\n");
            Field::sticky_web[get_player_from_position(def_pos)] = true;
            break;
        case Field_Objects::SPIKES:
            DEBUG_MSG("Spikes are scattered all around P" << get_player_from_position(def_pos) + 1 << "'s field\n");
            Field::spikes[get_player_from_position(def_pos)] += 1;

            if(Field::spikes[get_player_from_position(def_pos)] > 3)
                Field::spikes[get_player_from_position(def_pos)] = 3;

            break;
        case Field_Objects::TOXIC_SPIKES:
            DEBUG_MSG("Toxic pikes are scattered all around P" << get_player_from_position(def_pos) + 1 << "'s field\n");
            Field::toxic_spikes[get_player_from_position(def_pos)] += 1;
            break;
        case Field_Objects::LEECH_SEED:
            if(Field::active_pokes[def_pos]->get_type()[0] != PokeTypes::GRASS
            && Field::active_pokes[def_pos]->get_type()[1] != PokeTypes::GRASS
            && Field::leech_seed_positions[def_pos] == FIELD_POSITION::NO_POSITION)
            {
                DEBUG_MSG(Field::active_pokes[def_pos]->get_species() << " is now seeded\n");
                Field::leech_seed_positions[def_pos] = atk_pos;
            } else
            {
                DEBUG_MSG("Leech seed failed\n");
            }
            break;
        case Field_Objects::CLEAR:
            DEBUG_MSG("Clearing Field Objects\n");
            Field::reset_field_obj();
            break;
        case Field_Objects::WEATHER:
        case Field_Objects::TRICK_ROOM:
        default:
            ERR_MSG("Unhandled field object\n");
    }
}

bool Field::send_out(FIELD_POSITION pos, Pokemon* poke)
{
    if(Field::active_pokes[pos] == nullptr)
    {
        Field::active_pokes[pos] = poke;
        Field::active_pokes[pos]->set_active(true);
        return Field::handle_entrance(pos);
    }
    else
        ERR_MSG("Pokemon improperly returned before sending out " << poke->get_species() << std::endl);
}

void Field::return_poke(FIELD_POSITION pos)
{
    Field::leech_seed_positions[pos] = FIELD_POSITION::NO_POSITION;
    Field::active_pokes[pos]->set_active(false);
    Field::active_pokes[pos] = nullptr;
}

bool Field::handle_entrance(FIELD_POSITION pos)
{
    return handle_hazard_entrance(pos);
}

bool Field::handle_hazard_entrance(FIELD_POSITION pos)
{
    Players player = get_player_from_position(pos);
    float hp = Field::active_pokes[pos]->get_stat(STAT::HP), damage;
    if(Field::spikes[player] > 0 && Field::active_pokes[pos]->is_grounded())
    {
        damage = 1.0 / (10 - (2 * Field::spikes[player])) * hp;
        DEBUG_MSG(Field::active_pokes[pos]->get_species() << " is taking damage from spikes\n");
        if(!Field::active_pokes[pos]->deal_damage(damage))
            return false;
    }

    if(Field::toxic_spikes[player] >= 1 && Field::active_pokes[pos]->is_grounded())
    {
        if(Field::active_pokes[pos]->get_type()[0] == PokeTypes::POISON
        || Field::active_pokes[pos]->get_type()[1] == PokeTypes::POISON)
        {
            DEBUG_MSG(Field::active_pokes[pos]->get_species() << " absorbed the toxic spikes\n");
            Field::toxic_spikes[player] = 0;
        }
        else if(Field::toxic_spikes[player] > 1)
        {
            DEBUG_MSG(Field::active_pokes[pos]->get_species() << " was badly poisoned by toxic spikes\n");
            Field::active_pokes[pos]->set_status(STATUS::BADLY_POISONED);
        }
        else
        {
            DEBUG_MSG(Field::active_pokes[pos]->get_species() << " was poisoned by toxic spikes\n");
            Field::active_pokes[pos]->set_status(STATUS::POISONED);
        }
    }

    if(Field::stealth_rocks[player])
    {
        damage = 0.125 * hp * calculate_type_damage_modifier(Field::active_pokes[pos]->get_type(), PokeTypes::ROCK);
        DEBUG_MSG(Field::active_pokes[pos]->get_species() << " is taking damage from stealth rocks\n");
        if(!Field::active_pokes[pos]->deal_damage(damage))
            return false;
    }

    if(Field::sticky_web[player] && Field::active_pokes[pos]->is_grounded())
    {
        Field::active_pokes[pos]->stat_change(STAT::SPE, -1);
    }

    return true;
}

bool Field::active_open(FIELD_POSITION pos)
{
    return !Field::active_pokes[pos]->is_active();
}

void Field::reset()
{
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
        active_pokes[i] = nullptr;

    Field::reset_field_obj();

    Field::trick_room = false;
    Field::weather_state = Weather::CLEAR_SKIES;
    Field::terrain = Terrain::NO_TERRAIN;
}

void Field::reset_field_obj()
{
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        Field::stealth_rocks[get_player_from_position(i)] = false;
        Field::sticky_web[get_player_from_position(i)] = false;
        Field::spikes[get_player_from_position(i)] = 0;
        Field::toxic_spikes[get_player_from_position(i)] = 0;

        Field::leech_seed_positions[i] = FIELD_POSITION::NO_POSITION;
    }
}

void Field::print_field(bool detailed)
{
#ifdef DEBUG
    DEBUG_MSG("ACTIVE POKEMON: " << "\nPLAYER ONE\n");
    if(Field::active_pokes[Players::PLAYER_ONE]->is_active())
        Field::active_pokes[Players::PLAYER_ONE]->print_pokemon(detailed);
    else
        DEBUG_MSG("NONE\n");

    DEBUG_MSG("\nPLAYER TWO\n");
    if(Field::active_pokes[Players::PLAYER_TWO]->is_active())
        Field::active_pokes[Players::PLAYER_TWO]->print_pokemon(detailed);
    else
        DEBUG_MSG("NONE\n");

    if(detailed)
    {
        DEBUG_MSG("FIELD STATE P1:\n");
        if(Field::spikes[Players::PLAYER_ONE] > 0)
            DEBUG_MSG(Field::spikes[Players::PLAYER_ONE] << " layers of spikes\n");
        if(Field::toxic_spikes[Players::PLAYER_ONE])
            DEBUG_MSG("Toxic spikes\n");
        if(Field::stealth_rocks[Players::PLAYER_ONE])
            DEBUG_MSG("Stealth rocks\n");
        DEBUG_MSG("FIELD STATE P2:\n");
        if(Field::spikes[Players::PLAYER_TWO] > 0)
            DEBUG_MSG(Field::spikes[Players::PLAYER_TWO] << " layers of spikes\n");
        if(Field::toxic_spikes[Players::PLAYER_TWO])
            DEBUG_MSG("Toxic spikes\n");
        if(Field::stealth_rocks[Players::PLAYER_TWO])
            DEBUG_MSG("Stealth rocks\n");
        if(Field::trick_room)
            DEBUG_MSG("Trick room active\n");
        if(Field::terrain != Terrain::NO_TERRAIN)
            DEBUG_MSG(Field::terrain << " terrain is active\n");
        if(Field::weather_state != Weather::CLEAR_SKIES)
            DEBUG_MSG(Field::terrain << " weather is active\n");
    }
    DEBUG_MSG("\n");
#endif
}

bool Field::handle_end_turn_field_obj(FIELD_POSITION pos)
{
    if(Field::leech_seed_positions[pos] != FIELD_POSITION::NO_POSITION
    && Field::active_pokes[Field::leech_seed_positions[pos]]->is_alive())
    {
        int damage = Field::active_pokes[pos]->get_stat(STAT::HP) / 8.0;
        DEBUG_MSG(Field::active_pokes[Field::leech_seed_positions[pos]]->get_species()
             << " sapped some life from " << Field::active_pokes[Field::leech_seed_positions[pos]]->get_species() << std::endl);
        Field::active_pokes[Field::leech_seed_positions[pos]]->heal_damage(damage);
        return Field::active_pokes[pos]->deal_damage(damage);
    }

    return true;
}

Players get_player_from_position(int pos)
{
    if(pos == FIELD_POSITION::PLAYER_1_0
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    || pos == FIELD_POSITION::PLAYER_1_1
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    || pos == FIELD_POSITION::PLAYER_1_2
#endif
    )
        return Players::PLAYER_ONE;
    if(pos == FIELD_POSITION::PLAYER_2_0
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    || pos == FIELD_POSITION::PLAYER_2_1
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    || pos == FIELD_POSITION::PLAYER_2_2
#endif
       )
        return Players::PLAYER_TWO;

    ERR_MSG("Impossible field position\n");
}