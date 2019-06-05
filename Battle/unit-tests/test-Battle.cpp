//
// Created by slist on 5/30/19.
//

#include <gtest/gtest.h>
#include <Battle/Battle.h>
#include <Battle/Party.h>
#include <Pokemon/Pokemon.h>

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