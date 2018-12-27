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

bool Field::send_out(Players player, Pokemon poke)
{
    if(Field::active_open(player))
    {
        Field::active_pokes[player] = poke;
        Field::active_pokes[player].set_active(true);
        Field::handle_entrance(player);
        return true;
    }
    else
        return false;
}

void Field::handle_entrance(Players player)
{
    handle_hazard_entrance(player);
}

void Field::handle_hazard_entrance(Players player_num)
{
    int player = (int)player_num;
    int hp = Field::active_pokes[player].get_stat(STAT::HP);
    int damage;
    if(Field::spikes[player] > 0)
    {
        damage = 0.0625 * Field::spikes[player] * hp;
        std::cout << Field::active_pokes[player].get_species() << " is taking " << damage << " damage from spikes\n";
        Field::active_pokes[player].deal_damage(damage);
    }

    if(Field::toxic_spikes[player] == 1)
    {
        Field::active_pokes[player].set_status(STATUS::POISONED);
    }

    if(Field::toxic_spikes[player] > 1)
    {
        Field::active_pokes[player].set_status(STATUS::BADLY_POISONED);
    }

    if(Field::stealth_rocks[player])
    {
        damage = 0.125 * hp * calculate_type_damage_modifier(Field::active_pokes[player].get_type(), PokeTypes::ROCK);
        std::cout << Field::active_pokes[player].get_species() << " is taking " << damage << " damage from stealth rocks\n";
        Field::active_pokes[player].deal_damage(damage);
    }

    if(Field::sticky_web[player])
    {
        Field::active_pokes[player].stat_change(STAT::SPE, -1);
    }

}

bool Field::active_open(Players player)
{
    return !Field::active_pokes[player].is_active();
}

void Field::print_field(bool detailed)
{
    std::cout << "ACTIVE POKEMON: " << "\nPLAYER ONE\n";
    Field::active_pokes[Players::PLAYER_ONE].print_pokemon(detailed);
    std::cout << "\nPLAYER TWO\n";
    Field::active_pokes[Players::PLAYER_TWO].print_pokemon(detailed);
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
}





Players get_player_from_position(int pos)
{
    if(pos == FIELD_POSITION::PLAYER_1_0)
        return Players::PLAYER_ONE;
    if(pos == FIELD_POSITION::PLAYER_2_0)
        return Players::PLAYER_TWO;
}