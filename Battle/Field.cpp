//
// Created by slist on 12/14/18.
//

#include "Players.h"
#include "Field.h"
#include <iostream>

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
}

void Field::increase_field_obj(Field_Objects obj, FIELD_POSITION pos)
{
    switch(obj)
    {
        case Field_Objects::STEALTH_ROCKS:
            std::cout << "Rocks are scattered all around P" << get_player_from_position(pos) + 1 << "'s field\n";
            Field::stealth_rocks[get_player_from_position(pos)] = true;
            break;
        case Field_Objects::STICKY_WEB:
            std::cout << "Sticky web is scattered all around P" << get_player_from_position(pos) + 1 << "'s field\n";
            Field::sticky_web[get_player_from_position(pos)] = true;
            break;
        case Field_Objects::SPIKES:
            std::cout << "Spikes are scattered all around P" << get_player_from_position(pos) + 1 << "'s field\n";
            Field::spikes[get_player_from_position(pos)] += 1;

            if(Field::spikes[get_player_from_position(pos)] > 3)
                Field::spikes[get_player_from_position(pos)] = 3;

            break;
        case Field_Objects::TOXIC_SPIKES:
            std::cout << "Toxic pikes are scattered all around P" << get_player_from_position(pos) + 1 << "'s field\n";
            Field::toxic_spikes[get_player_from_position(pos)] += 1;
            break;
        case Field_Objects::WEATHER:
        case Field_Objects::TRICK_ROOM:
        default:
            assert(0);
    }
}

bool Field::send_out(FIELD_POSITION pos, Pokemon poke)
{
    if(Field::active_open(pos))
    {
        Field::active_pokes[pos] = poke;
        Field::active_pokes[pos].set_active(true);
        return Field::handle_entrance(pos);
    }
    else
        return false;
}

void Field::return_poke(FIELD_POSITION pos)
{
    Field::active_pokes[pos].set_active(false);
}

bool Field::handle_entrance(FIELD_POSITION pos)
{
    return handle_hazard_entrance(pos);
}

bool Field::handle_hazard_entrance(FIELD_POSITION pos)
{
    Players player = get_player_from_position(pos);
    float hp = Field::active_pokes[pos].get_stat(STAT::HP), damage;
    if(Field::spikes[player] > 0)
    {
        damage = 1.0 / (2 * (Field::spikes[player] + 1)) * hp;
        std::cout << Field::active_pokes[pos].get_species() << " is taking damage from spikes\n";
        if(!Field::active_pokes[pos].deal_damage(damage))
            return false;
    }

    if(Field::toxic_spikes[player] == 1)
    {
        std::cout << Field::active_pokes[pos].get_species() << " was poisoned by toxic spikes\n";
        Field::active_pokes[pos].set_status(STATUS::POISONED);
    }

    if(Field::toxic_spikes[player] > 1)
    {
        Field::active_pokes[pos].set_status(STATUS::BADLY_POISONED);
    }

    if(Field::stealth_rocks[player])
    {
        damage = 0.125 * hp * calculate_type_damage_modifier(Field::active_pokes[pos].get_type(), PokeTypes::ROCK);
        std::cout << Field::active_pokes[pos].get_species() << " is taking damage from stealth rocks\n";
        if(!Field::active_pokes[pos].deal_damage(damage))
            return false;
    }

    if(Field::sticky_web[player])
    {
        Field::active_pokes[pos].stat_change(STAT::SPE, -1);
    }

}

bool Field::active_open(FIELD_POSITION pos)
{
    return !Field::active_pokes[pos].is_active();
}

void Field::reset()
{
    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
        active_pokes[i].set_active(false);

    Field::stealth_rocks[Players::PLAYER_ONE] = false;
    Field::stealth_rocks[Players::PLAYER_TWO] = false;
    Field::sticky_web[Players::PLAYER_ONE] = false;
    Field::sticky_web[Players::PLAYER_TWO] = false;
    Field::spikes[Players::PLAYER_ONE] = 0;
    Field::spikes[Players::PLAYER_TWO] = 0;
    Field::toxic_spikes[Players::PLAYER_ONE] = 0;
    Field::toxic_spikes[Players::PLAYER_TWO] = 0;
    Field::trick_room = false;
    Field::weather_state = Weather::CLEAR_SKIES;
    Field::terrain = Terrain::NO_TERRAIN;
}

void Field::print_field(bool detailed)
{
    std::cout << "ACTIVE POKEMON: " << "\nPLAYER ONE\n";
    if(Field::active_pokes[Players::PLAYER_ONE].is_active())
        Field::active_pokes[Players::PLAYER_ONE].print_pokemon(detailed);
    else
        std::cout << "NONE\n";

    std::cout << "\nPLAYER TWO\n";
    if(Field::active_pokes[Players::PLAYER_TWO].is_active())
        Field::active_pokes[Players::PLAYER_TWO].print_pokemon(detailed);
    else
        std::cout << "NONE\n";

    if(detailed)
    {
        std::cout << "FIELD STATE P1:\n";
        if(Field::spikes[Players::PLAYER_ONE] > 0)
            std::cout << Field::spikes[Players::PLAYER_ONE] << " layers of spikes\n";
        if(Field::toxic_spikes[Players::PLAYER_ONE])
            std::cout << "Toxic spikes\n";
        if(Field::stealth_rocks[Players::PLAYER_ONE])
            std::cout << "Stealth rocks\n";
        std::cout << "FIELD STATE P2:\n";
        if(Field::spikes[Players::PLAYER_TWO] > 0)
            std::cout << Field::spikes[Players::PLAYER_TWO] << " layers of spikes\n";
        if(Field::toxic_spikes[Players::PLAYER_TWO])
            std::cout << "Toxic spikes\n";
        if(Field::stealth_rocks[Players::PLAYER_TWO])
            std::cout << "Stealth rocks\n";
        if(Field::trick_room)
            std::cout << "Trick room active\n";
        if(Field::terrain != Terrain::NO_TERRAIN)
            std::cout << Field::terrain << " terrain is active\n";
        if(Field::weather_state != Weather::CLEAR_SKIES)
            std::cout << Field::terrain << " weather is active\n";
    }
    std::cout << "\n";
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

    assert(0);
}