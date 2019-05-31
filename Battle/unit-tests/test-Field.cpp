//
// Created by slist on 5/22/19.
//

#include <gtest/gtest.h>
#include <Battle/Field.h>
#include <Battle/Players.h>
#include <Battle/FieldPositions.h>

TEST(test_get_Field_Positions, p1)
{
    EXPECT_EQ(Players::PLAYER_ONE, get_player_from_position(PLAYER_1_0));
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(Players::PLAYER_ONE, get_player_from_position(PLAYER_1_1));
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(Players::PLAYER_ONE, get_player_from_position(PLAYER_1_2));
#endif
}

TEST(test_get_Field_Positions, p2)
{
    EXPECT_EQ(Players::PLAYER_TWO, get_player_from_position(PLAYER_2_0));
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(Players::PLAYER_TWO, get_player_from_position(PLAYER_2_1));
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(Players::PLAYER_TWO, get_player_from_position(PLAYER_2_2));
#endif
}

TEST(test_modify_field_obj, STEALTH_ROCKS)
{
    Field f;
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.stealth_rocks[Players::PLAYER_TWO]);
}

TEST(test_modify_field_obj, SPIKES)
{
    Field f;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.spikes[Players::PLAYER_TWO], 1);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.spikes[Players::PLAYER_TWO], 2);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.spikes[Players::PLAYER_TWO], 3);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.spikes[Players::PLAYER_TWO], 3);
}

TEST(test_modify_field_obj, STICKY_WEB)
{
    Field f;
    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.sticky_web[Players::PLAYER_TWO]);
}

TEST(test_modify_field_obj, TOXIC_SPIKES)
{
    Field f;
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.toxic_spikes[Players::PLAYER_TWO]);
}

TEST(test_modify_field_obj, LEECH_SEED_no_grass)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.leech_seed_positions[PLAYER_2_0], PLAYER_1_0);
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_2_0);
    EXPECT_EQ(f.leech_seed_positions[PLAYER_2_0], PLAYER_1_0);
}

TEST(test_modify_field_obj, LEECH_SEED_grass)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::GRASS, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0);
    EXPECT_EQ(f.leech_seed_positions[PLAYER_2_0], FIELD_POSITION::NO_POSITION);
}

TEST(test_modify_field_obj, LEECH_SEED_no_poke)
{
    Field f;
    f.active_pokes[PLAYER_2_0] = nullptr;
    EXPECT_DEATH(f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0), "");
}

TEST(test_modify_field_obj, WEATHER)
{
    Field f;
    f.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::RAIN);
    EXPECT_EQ(f.weather_state, Weather::RAIN);
}

TEST(test_modify_field_obj, TRICK_ROOM)
{
    Field f;
    EXPECT_DEATH(f.modify_field_obj(FieldObjects::TRICK_ROOM, PLAYER_2_0, PLAYER_1_0), "");
}

TEST(test_modify_field_obj, CLEAR)
{
    Field f;
    Pokemon p[FIELD_POSITION::NUM_POSITIONS];

    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        p[i].create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
        f.active_pokes[i] = &p[i];
        f.modify_field_obj(FieldObjects::LEECH_SEED, static_cast<FIELD_POSITION>(i), PLAYER_1_0);
    }

    f.modify_field_obj(FieldObjects::CLEAR, PLAYER_1_0, PLAYER_2_0);

    EXPECT_FALSE(f.stealth_rocks[0]);
    EXPECT_FALSE(f.stealth_rocks[1]);

    EXPECT_FALSE(f.sticky_web[0]);
    EXPECT_FALSE(f.sticky_web[1]);

    EXPECT_EQ(f.spikes[0], 0);
    EXPECT_EQ(f.spikes[1], 0);

    EXPECT_EQ(f.toxic_spikes[0], 0);
    EXPECT_EQ(f.toxic_spikes[1], 0);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        EXPECT_EQ(f.leech_seed_positions[i], NO_POSITION);
    }
}

TEST(test_position_alive, nullptr)
{
    Field f;
    EXPECT_FALSE(f.position_alive(PLAYER_2_0));
}

TEST(test_position_alive, fainted)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    p.faint_poke();
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_FALSE(f.position_alive(PLAYER_2_0));
}

TEST(test_position_alive, alive)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_TRUE(f.position_alive(PLAYER_2_0));
}

TEST(test_field_send_out, position_filled)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = new(Pokemon);
    EXPECT_DEATH(f.send_out(PLAYER_2_0, &p), "");
}

TEST(test_field_send_out, faint)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.send_out(PLAYER_2_0, &p));
}

TEST(test_field_send_out, happy_path)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.send_out(PLAYER_2_0, &p));
    EXPECT_TRUE(f.active_pokes[PLAYER_2_0]->is_active());
    EXPECT_TRUE(f.active_pokes[PLAYER_2_0]->first_turn);
}

TEST(test_field_return_poke, happy_path)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.send_out(PLAYER_2_0, &p);
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0);
    f.return_poke(PLAYER_2_0);
    EXPECT_FALSE(p.is_active());
    EXPECT_EQ(f.active_pokes[PLAYER_2_0], nullptr);
    EXPECT_EQ(f.leech_seed_positions[PLAYER_2_0], FIELD_POSITION::NO_POSITION);
}

TEST(test_field_return_poke, no_poke)
{
    Field f;
    EXPECT_DEATH(f.return_poke(PLAYER_2_0), "");
}

TEST(test_handle_entrance, happy_path)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_TRUE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_entrance, faint_spikes)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, faint_spikes_1)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, live_spikes_1)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, faint_spikes_2)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 16);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, live_spikes_2)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 17);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, faint_spikes_3)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 25);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, live_spikes_3)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 26);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, toxic_spikes_1)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 26);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.handle_entrance(PLAYER_2_0);
    EXPECT_EQ(f.active_pokes[PLAYER_2_0]->get_status(), STATUS::POISONED);
}

TEST(test_handle_hazard_entrance, toxic_spikes_2)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 26);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.handle_entrance(PLAYER_2_0);
    EXPECT_EQ(f.active_pokes[PLAYER_2_0]->get_status(), STATUS::BADLY_POISONED);
}

TEST(test_handle_hazard_entrance, toxic_spikes_flying_poke)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::FLYING, PokeTypes::NO_TYPE, Natures::QUIRKY, 26);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.handle_entrance(PLAYER_2_0);
    EXPECT_EQ(f.active_pokes[PLAYER_2_0]->get_status(), STATUS::NO_STATUS);
}

TEST(test_handle_hazard_entrance, toxic_spikes_poison_poke)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::POISON, PokeTypes::NO_TYPE, Natures::QUIRKY, 26);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);
    f.handle_entrance(PLAYER_2_0);
    EXPECT_EQ(f.active_pokes[PLAYER_2_0]->get_status(), STATUS::NO_STATUS);
    EXPECT_EQ(f.toxic_spikes[PLAYER_TWO], 0);
}

TEST(test_handle_hazard_entrance, stealth_rocks_faint)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_handle_hazard_entrance, stealth_rocks_alive)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = &p;
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.handle_entrance(PLAYER_2_0));
}

TEST(test_active_open, pos_open)
{
    Field f;
    EXPECT_TRUE(f.active_open(PLAYER_2_0));
}

TEST(test_active_open, pos_open_with_poke)
{
    Field f;
    Pokemon p;
    p = *new(Pokemon);
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_TRUE(f.active_open(PLAYER_2_0));
}

TEST(test_active_open, pos_closed)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_FALSE(f.active_open(PLAYER_2_0));
}

TEST(test_handle_end_turn_field_obj, happy_path)
{
    Field f;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::GRASS, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_2_0] = &p;
    EXPECT_TRUE(f.handle_end_turn_field_obj(PLAYER_2_0));
}


TEST(test_handle_end_turn_field_obj, LEECH_SEED_live)
{
    Field f;
    Pokemon p1, p2;
    p1.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    p2.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    f.active_pokes[PLAYER_1_0] = &p1;
    f.active_pokes[PLAYER_2_0] = &p2;
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0);
    EXPECT_TRUE(f.handle_end_turn_field_obj(PLAYER_2_0));
    EXPECT_EQ(f.active_pokes[PLAYER_1_0]->get_current_hp(), 25);
    EXPECT_EQ(f.active_pokes[PLAYER_2_0]->get_current_hp(), 1);
}

TEST(test_handle_end_turn_field_obj, LEECH_SEED_faint)
{
    Field f;
    Pokemon p1, p2;
    p1.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
    p2.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 12);
    f.active_pokes[PLAYER_1_0] = &p1;
    f.active_pokes[PLAYER_2_0] = &p2;
    f.modify_field_obj(FieldObjects::LEECH_SEED, PLAYER_2_0, PLAYER_1_0);
    EXPECT_FALSE(f.handle_end_turn_field_obj(PLAYER_2_0));
    EXPECT_EQ(f.active_pokes[PLAYER_1_0]->get_current_hp(), 25);
}

TEST(test_reset_field_obj, happy)
{
    Field f;
    Pokemon p[FIELD_POSITION::NUM_POSITIONS];

    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        p[i].create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
        f.active_pokes[i] = &p[i];
        f.modify_field_obj(FieldObjects::LEECH_SEED, static_cast<FIELD_POSITION>(i), PLAYER_1_0);
    }

    f.reset_field_obj();

    EXPECT_FALSE(f.stealth_rocks[0]);
    EXPECT_FALSE(f.stealth_rocks[1]);

    EXPECT_FALSE(f.sticky_web[0]);
    EXPECT_FALSE(f.sticky_web[1]);

    EXPECT_EQ(f.spikes[0], 0);
    EXPECT_EQ(f.spikes[1], 0);

    EXPECT_EQ(f.toxic_spikes[0], 0);
    EXPECT_EQ(f.toxic_spikes[1], 0);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        EXPECT_EQ(f.leech_seed_positions[i], NO_POSITION);
    }
}

TEST(test_reset_field, happy)
{
    Field f;
    Pokemon p[FIELD_POSITION::NUM_POSITIONS];

    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STICKY_WEB, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::TOXIC_SPIKES, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_1_0, PLAYER_2_0);
    f.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_2_0, PLAYER_1_0);

    f.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::RAIN);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        p[i].create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13);
        f.active_pokes[i] = &p[i];
        f.modify_field_obj(FieldObjects::LEECH_SEED, static_cast<FIELD_POSITION>(i), PLAYER_1_0);
    }

    f.reset();

    EXPECT_FALSE(f.stealth_rocks[0]);
    EXPECT_FALSE(f.stealth_rocks[1]);

    EXPECT_FALSE(f.sticky_web[0]);
    EXPECT_FALSE(f.sticky_web[1]);

    EXPECT_EQ(f.spikes[0], 0);
    EXPECT_EQ(f.spikes[1], 0);

    EXPECT_EQ(f.toxic_spikes[0], 0);
    EXPECT_EQ(f.toxic_spikes[1], 0);

    EXPECT_EQ(f.weather_state, Weather::CLEAR_SKIES);
    EXPECT_EQ(f.weather_turns, 0);

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        EXPECT_EQ(f.active_pokes[i], nullptr);
        EXPECT_EQ(f.leech_seed_positions[i], NO_POSITION);
    }
}

TEST(test_handle_end_turn_weather, turn_countdown)
{
    Field f;
    f.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::RAIN);
    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::RAIN);
    EXPECT_EQ(f.weather_turns, 4);

    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::RAIN);
    EXPECT_EQ(f.weather_turns, 3);

    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::RAIN);
    EXPECT_EQ(f.weather_turns, 2);

    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::RAIN);
    EXPECT_EQ(f.weather_turns, 1);

    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::CLEAR_SKIES);
    EXPECT_EQ(f.weather_turns, 0);

    f.handle_end_turn_weather();
    EXPECT_EQ(f.weather_state, Weather::CLEAR_SKIES);
    EXPECT_EQ(f.weather_turns, 0);
}