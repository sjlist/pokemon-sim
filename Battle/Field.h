//
// Created by slist on 12/14/18.
//

#ifndef POKEMON_SIM_FIELD_H
#define POKEMON_SIM_FIELD_H

#include "Pokemon/Pokemon.h"
#include "Players.h"

enum FIELD_POSITION
{
    PLAYER_1_0 = 0,
    //PLAYER_1_1,
    PLAYER_2_0,
    //PLAYER_2_1,
    NUM_POSITIONS,
    NO_POSITION
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
    bool send_out(FIELD_POSITION pos, Pokemon poke);
    void return_poke(FIELD_POSITION pos);

    Pokemon active_pokes [2];

    //entrance hazards
    bool stealth_rocks [2], sticky_web [2];
    int spikes [2], toxic_spikes [2];

    bool trick_room;
    Weather weather_state;
    Terrain terrain;

    void print_field(bool detailed=false);
private:
    void handle_entrance(FIELD_POSITION pos);
    void handle_hazard_entrance(FIELD_POSITION pos);
    bool active_open(FIELD_POSITION pos);
};

Players get_player_from_position(int pos);

#endif //POKEMON_SIM_FIELD_H
