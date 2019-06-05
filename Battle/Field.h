//
// Created by slist on 12/14/18.
//

#ifndef POKEMON_SIM_FIELD_H
#define POKEMON_SIM_FIELD_H

#include <gtest/gtest_prod.h>

#include <Battle/FieldObjects.h>
#include <Battle/FieldPositions.h>
#include <Battle/Players.h>
#include <Pokemon/Pokemon.h>

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

    void reset();

    bool send_out(FIELD_POSITION pos, Pokemon* poke);
    void return_poke(FIELD_POSITION pos);

    void modify_field_obj(FieldObjects obj, FIELD_POSITION pos, FIELD_POSITION atk_pos, int field_value = 0);
    bool handle_end_turn_field_obj(FIELD_POSITION pos);
    void handle_end_turn_weather();
    bool position_alive(FIELD_POSITION pos);

    Pokemon *active_pokes [NUM_POSITIONS];
    FIELD_POSITION leech_seed_positions [NUM_POSITIONS];

    //entrance hazards
    bool stealth_rocks [2], sticky_web [2];
    int spikes [2], toxic_spikes [2];

    bool trick_room;
    Weather weather_state;
    int weather_turns;
    Terrain terrain;

    void print_field(bool detailed=false);

protected:
    bool handle_entrance(FIELD_POSITION pos);
    void reset_field_obj();
    bool handle_hazard_entrance(FIELD_POSITION pos);
    bool active_open(FIELD_POSITION pos);

private:

};

Players get_player_from_position(FIELD_POSITION pos);

#endif //POKEMON_SIM_FIELD_H
