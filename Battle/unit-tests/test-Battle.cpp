//
// Created by slist on 5/30/19.
//

#include <gtest/gtest.h>
#include <Battle/Battle.h>
#include <Battle/Party.h>
#include <Pokemon/Pokemon.h>

#include <map>

#include "probability-helpers.h"

class BattleTest: public Battle
{
    FRIEND_TEST(test_get_party, happy);
    FRIEND_TEST(test_battle_send_out, reset_badly_poisoned_turns);
    FRIEND_TEST(test_battle_send_out, faint);
    FRIEND_TEST(test_battle_send_out, poke_already_fainted);
    FRIEND_TEST(test_battle_send_out, poke_already_active);
    FRIEND_TEST(test_battle_swap_poke, happy);
    FRIEND_TEST(test_battle_swap_poke, faint);
    FRIEND_TEST(test_battle_swap_poke, poke_already_fainted);
    FRIEND_TEST(test_battle_swap_poke, poke_already_active);
    FRIEND_TEST(test_get_move_power, happy);
    FRIEND_TEST(test_get_move_power, gyro_ball);
    FRIEND_TEST(test_get_move_power, bad_move_power);
    FRIEND_TEST(test_calc_damage_modifiers, stab);
    FRIEND_TEST(test_calc_damage_modifiers, super_effective);
    FRIEND_TEST(test_calc_damage_modifiers, super_effective_stab);
    FRIEND_TEST(test_calc_damage_modifiers, not_very_effective);
    FRIEND_TEST(test_calc_damage_modifiers, not_very_effective_stab);
    FRIEND_TEST(test_calc_damage_modifiers, normal);
    FRIEND_TEST(test_calc_damage_modifiers, weather_boost);
    FRIEND_TEST(test_calc_damage_modifiers, weather_reduce);
    FRIEND_TEST(test_calc_damage_modifiers, weather_extremes);
    FRIEND_TEST(test_calc_damage_modifiers, burned_physical);
    FRIEND_TEST(test_calc_damage_modifiers, burned_special);
    FRIEND_TEST(test_calc_damage_modifiers, 2_targets);
    FRIEND_TEST(test_calc_damage_modifiers, grounded);
    FRIEND_TEST(test_calc_damage_modifiers, crit);
    FRIEND_TEST(test_has_lost, no_loss);
    FRIEND_TEST(test_has_lost, lost);
    FRIEND_TEST(test_roll_chance, 5);
    FRIEND_TEST(test_roll_chance, 20);
    FRIEND_TEST(test_calculate_damage_dealt, claw);
    FRIEND_TEST(test, sad);
};

TEST(test_get_party, happy)
{
    BattleTest b;
    Party P_orig, *P_new;

    for(int i = 0; i < 6; i++)
        P_orig.party_pokes[i].create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, Natures::QUIRKY, 13 + i);

    b.Parties[PLAYER_ONE] = P_orig;
    P_new = b.get_party(Players::PLAYER_ONE);
    for(int i = 0; i < 6; i++)
        EXPECT_EQ(P_new->party_pokes[i].get_current_hp(), P_orig.party_pokes[i].get_current_hp());
}

TEST(test_battle_send_out, bad_pos)
{
    BattleTest b;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, -2), "");
}

TEST(test_battle_send_out, no_pokes_left)
{
    BattleTest b;
    EXPECT_EQ(b.send_out(PLAYER_1_0, -1), Attack_Result::NO_ATTACK);
}

TEST(test_battle_send_out, reset_badly_poisoned_turns)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.set_active(false);
    p.set_status(STATUS::BADLY_POISONED);
    p.status_turns = 12;
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_EQ(b.send_out(PLAYER_1_0, 0), Attack_Result::HIT);
    EXPECT_EQ(b.active_field.active_pokes[PLAYER_1_0]->get_status(), STATUS::BADLY_POISONED);
    EXPECT_EQ(b.active_field.active_pokes[PLAYER_1_0]->status_turns, 0);
}

TEST(test_battle_send_out, faint)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.set_active(false);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    b.active_field.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_1_0, PLAYER_2_0);
    EXPECT_EQ(b.send_out(PLAYER_1_0, 0), Attack_Result::FAINT);
    EXPECT_FALSE(b.Parties[PLAYER_ONE].party_pokes[0].is_alive());
}

TEST(test_battle_send_out, poke_already_fainted)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.faint_poke();
    p.set_active(false);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, 0), "");
}

TEST(test_battle_send_out, poke_already_active)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, 0), "");
}

TEST(test_battle_swap_poke, bad_pos)
{
    BattleTest b;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, -2), "");
}

TEST(test_battle_swap_poke, no_pokes_left)
{
    BattleTest b;
    EXPECT_EQ(b.send_out(PLAYER_1_0, -1), Attack_Result::NO_ATTACK);
}

TEST(test_battle_swap_poke, happy)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.set_active(false);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_EQ(b.send_out(PLAYER_1_0, 0), Attack_Result::HIT);
}

TEST(test_battle_swap_poke, faint)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.set_active(false);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    b.active_field.modify_field_obj(FieldObjects::STEALTH_ROCKS, PLAYER_1_0, PLAYER_2_0);
    EXPECT_EQ(b.send_out(PLAYER_1_0, 0), Attack_Result::FAINT);
    EXPECT_FALSE(b.Parties[PLAYER_ONE].party_pokes[0].is_alive());
}

TEST(test_battle_swap_poke, poke_already_fainted)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.faint_poke();
    p.set_active(false);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, 0), "");
}

TEST(test_battle_swap_poke, poke_already_active)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;
    EXPECT_DEATH(b.send_out(PLAYER_1_0, 0), "");
}

TEST(test_get_move_power, happy)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.moves[0].load_move("Unit-Test/Test_Move_1");
    b.active_field.active_pokes[PLAYER_1_0] = &p;
    EXPECT_EQ(b.get_move_power(PLAYER_1_0, PLAYER_2_0, &b.active_field.active_pokes[0]->moves[0]), 75);
}

TEST(test_get_move_power, gyro_ball)
{
    BattleTest b;
    Pokemon p1, p2;
    p1.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, JOLLY, 10);
    p1.moves[0].load_move("Gyro_Ball");
    b.active_field.active_pokes[PLAYER_1_0] = &p1;
    b.active_field.active_pokes[PLAYER_2_0] = &p2;
    EXPECT_EQ(b.get_move_power(PLAYER_1_0, PLAYER_2_0, &b.active_field.active_pokes[0]->moves[0]), 28);
}

TEST(test_get_move_power, bad_move_power)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.moves[0].load_move("Gyro_Ball");
    b.active_field.active_pokes[PLAYER_1_0] = &p;
    EXPECT_DEATH(b.get_move_power(PLAYER_1_0, PLAYER_2_0, &b.active_field.active_pokes[0]->moves[0]), "");
}

TEST(test_calc_damage_modifiers, normal)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 1;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Fire_Fang");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, stab)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 1.5;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::DARK, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Crunch");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, super_effective)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 2;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::PSYCHIC, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Crunch");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, super_effective_stab)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 3;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::DARK, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::PSYCHIC, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Crunch");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, not_very_effective)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 0.5;
    int power = 100;
    vector<float> mults (20, 1);

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::DARK, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Crunch");

    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, not_very_effective_stab)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 0.75;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::DARK, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::DARK, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Crunch");

    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, weather_boost)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m1, m2;
    float expected_mod = 1.5;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m1.load_move("Scald");
    m2.load_move("Fire_Fang");

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::RAIN);
    b.calculate_damage_modifier(&mults, &m1, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
    mults.clear();

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::HARSH_SUNLIGHT);
    b.calculate_damage_modifier(&mults, &m2, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, weather_reduce)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m1, m2;
    float expected_mod = 0.5;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m1.load_move("Scald");
    m2.load_move("Fire_Fang");

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::RAIN);
    b.calculate_damage_modifier(&mults, &m2, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
    mults.clear();

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::HARSH_SUNLIGHT);
    b.calculate_damage_modifier(&mults, &m1, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, weather_extremes)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m1, m2;
    float expected_mod_1 = 1.5;
    float expected_mod_2 = 0;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m1.load_move("Scald");
    m2.load_move("Fire_Fang");

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::HEAVY_RAIN);
    b.calculate_damage_modifier(&mults, &m1, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod_1 * power);
    mults.clear();

    b.calculate_damage_modifier(&mults, &m2, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod_2 * power);
    mults.clear();

    b.active_field.modify_field_obj(FieldObjects::WEATHER, PLAYER_2_0, PLAYER_1_0, Weather::EXTREMELY_HARSH_SUNLIGHT);
    b.calculate_damage_modifier(&mults, &m2, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod_1 * power);
    mults.clear();

    b.calculate_damage_modifier(&mults, &m1, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod_2 * power);
    mults.clear();
}

TEST(test_calc_damage_modifiers, burned_physical)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 0.5;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p1.set_status(BURNED);
    m.load_move("Fire_Fang");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, burned_special)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 1;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p1.set_status(BURNED);
    m.load_move("Scald");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, 2_targets)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 0.75;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Scald");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 2, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, grounded)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 0;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::FLYING, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Earthquake");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, false);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_calc_damage_modifiers, crit)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    float expected_mod = 1.5;
    int power = 100;
    vector<float> mults;

    p1.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p2.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    m.load_move("Earthquake");
    b.calculate_damage_modifier(&mults, &m, &p1, &p2, 1, true);
    EXPECT_EQ(b.do_chain_mult(power, &mults), expected_mod * power);
}

TEST(test_has_lost, no_loss)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    b.Parties[PLAYER_ONE].party_pokes[0] = p;

    EXPECT_FALSE(b.has_lost(PLAYER_ONE));
}

TEST(test_has_lost, lost)
{
    BattleTest b;
    Pokemon p;
    p.create_test_pokemon(PokeTypes::NORMAL, PokeTypes::NO_TYPE, QUIRKY, 10);
    p.faint_poke();
    b.Parties[PLAYER_ONE].party_pokes[0] = p;

    EXPECT_TRUE(b.has_lost(PLAYER_ONE));
}

TEST(test_roll_chance, 5)
{
    BattleTest b;
    int num_runs = get_num_samples(2);
    float res[2] = {0, 0}, chance = 0.05;

    for(int i = 0; i < num_runs; i++)
    {
        if(b.roll_chance(chance))
        {
            res[1]++;
        }
        else
            res[0]++;
    }

    res[0] = abs(res[0]/num_runs - (1-chance)) * 100;
    res[1] = abs(res[1]/num_runs - chance) * 100;

    EXPECT_LE(abs(res[0]), THRESHOLD*100);
    EXPECT_LE(abs(res[1]), THRESHOLD*100);
}

TEST(test_roll_chance, 20)
{
    BattleTest b;
    int num_runs = get_num_samples(2);
    float res[2] = {0, 0}, chance = 0.2;

    for(int i = 0; i < num_runs; i++)
    {
        if(b.roll_chance(chance))
        {
            res[1]++;
        }
        else
            res[0]++;
    }

    res[0] = abs(res[0]/num_runs - (1-chance)) * 100;
    res[1] = abs(res[1]/num_runs - chance) * 100;

    EXPECT_LE(abs(res[0]), THRESHOLD*100);
    EXPECT_LE(abs(res[1]), THRESHOLD*100);
}

TEST(test_calculate_damage_dealt, claw)
{
    BattleTest b;
    Pokemon p1, p2;
    Move m;
    vector<float> mults (20, 1);
    map<float, float> results;
    map<float, float> expected_outcomes = {{122, 1}, {126, 2}, {128, 2},
                                           {132, 2}, {134, 2}, {138, 2},
                                           {140, 2}, {144, 2}, {146, 1}};
    int damage, N = expected_outcomes.size(), num_runs = get_num_samples(N);

    p1.create_test_pokemon(PokeTypes::DRAGON, PokeTypes::GROUND, JOLLY, 100, "Garchomp");
    p2.create_test_pokemon(PokeTypes::DRAGON, PokeTypes::GROUND, JOLLY, 100, "Garchomp");
    b.active_field.active_pokes[PLAYER_1_0] = &p1;
    b.active_field.active_pokes[PLAYER_2_0] = &p2;

    m.load_move("Dragon_Claw");

    b.calculate_damage_modifier(&mults, &m, b.active_field.active_pokes[PLAYER_1_0], b.active_field.active_pokes[PLAYER_2_0], 1, false);

    for(int i = 0; i < num_runs; i++)
    {
        damage = b.calculate_damage_dealt(p1.get_level(), b.get_move_power(PLAYER_1_0, PLAYER_2_0, &m), p1.get_stat(STAT::ATK), p2.get_stat(STAT::DEF), &mults);
        results[damage]++;
        mults.erase(mults.begin());
    }

    //NORMALIZE!
    for (auto const& x : expected_outcomes)
    {
        expected_outcomes[x.first] = expected_outcomes[x.first] / 16.0;
        results[x.first] = results[x.first] / static_cast<float>(num_runs);
        EXPECT_LE(abs(results[x.first] - expected_outcomes[x.first]), THRESHOLD);
    }
}