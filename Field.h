//
// Created by slist on 12/14/18.
//

#ifndef POKEMON_SIM_FIELD_H
#define POKEMON_SIM_FIELD_H

#include "Pokemon.h"
#include "Players.h"

enum FIELD_POSITION
{
    PLAYER_1_0 = 0,
    PLAYER_1_1,
    PLAYER_2_0,
    PLAYER_2_1
};

enum Terrain
{
    NO_TERRAIN = 0,
    ELECTRIC_TERRAIN,
    GRASSY_TERRAIN,
    MISTY_TERRAIN,
    PSYCHIC_TERRAIN,
    GENESIS_SUPERNOVA,
    NUM_TERRAINS
};

enum Weather
{
    CLEAR_SKIES = 0,
    HARSH_SUNLIGHT,
    EXTREMELY_HARSH_SUNLIGHT,
    RAIN,
    HEAVY_RAIN,
    SANDSTORM,
    HAIL,
    MYSTERIOUS_AIR_CURRENT
};

class Field {
public:
    Field();
    bool send_out(Players player, Pokemon poke);

    Pokemon active_pokes [2];
    bool stealth_rocks, toxic_spikes, spikes, trick_room;
    Weather weather_state;
    Terrain terrain;

    void print_field();
private:
    bool active_open(Players player);
};


#endif //POKEMON_SIM_FIELD_H
