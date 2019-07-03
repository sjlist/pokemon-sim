//
// Created by slist on 6/27/19.
//

#include <gtest/gtest.h>
#include <Battle/BattleStateMachine.h>
#include <Battle/BattleMessage.h>
using namespace std;

class BSMTest: public BattleStateMachine
{
public:
    void load_test_teams(string team1, string team2);

private:
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_same_prio);
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks);
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks_swap);
};

void BSMTest::load_test_teams(string team1, string team2)
{
    BattleMessage m;
    EXPECT_EQ(BSMTest::state, BattleState::BATTLE_IDLE);
    BSMTest::run(m);
    EXPECT_EQ(BSMTest::state, BattleState::BATTLE_INIT);
    m.move_command = Commands::COMMAND_TEAM_CHOICE;
    m.team_name = team1;
    m.pos = PLAYER_1_0;
    BSMTest::run(m);
    m.team_name = team2;
    m.pos = PLAYER_2_0;
    BSMTest::run(m);
    EXPECT_EQ(BSMTest::state, BattleState::BATTLE_START);
    m.move_command = Commands::COMMAND_SWAP;
    m.pos = PLAYER_1_0;
    m.reserve_poke = 0;
    BSMTest::run(m);
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    m.pos = PLAYER_1_1;
    m.reserve_poke = 2;
    BSMTest::run(m);
#endif
    m.pos = PLAYER_2_0;
    m.reserve_poke = 0;
    BSMTest::run(m);
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    m.pos = PLAYER_2_1;
    m.reserve_poke = 2;
    BSMTest::run(m);
#endif
    EXPECT_EQ(BSMTest::state, BattleState::ACTION_REQUEST);
}

TEST(test_sort_message_stack, no_speed_ties_same_prio) {
    //Team 2 has 0 speed IVs team 1 has 31. All else held equal. So in priority ties between pokes os same species, team1 wins
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2);

    //All moves have the same priority
    m.move_command = Commands::COMMAND_ATTACK;
    m.move_num = 0;

    m.pos = PLAYER_1_0;
    m.target_pos = PLAYER_2_0;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_1_1;
    m.target_pos = PLAYER_2_1;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_2_0;
    m.target_pos = PLAYER_1_0;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_2_1;
    m.target_pos = PLAYER_1_1;
    BSM.turn_messages.push_back(m);

    BSM.sort_message_stack();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_0);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_0);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_1);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_1);
    BSM.turn_messages.pop_back();
}

TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks)
{
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2);

    //Pos 1_1 and 2_1 are using a higher priority move than 1_0, 2_0
    m.move_command = Commands::COMMAND_ATTACK;
    m.move_num = 2;

    m.pos = PLAYER_1_0;
    m.target_pos = PLAYER_2_0;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_1_1;
    m.target_pos = PLAYER_2_1;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_2_0;
    m.target_pos = PLAYER_1_0;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_2_1;
    m.target_pos = PLAYER_1_1;
    BSM.turn_messages.push_back(m);

    BSM.sort_message_stack();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_1);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_1);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_0);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_0);
    BSM.turn_messages.pop_back();
}

TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks_swap)
{
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2);
    m.move_command = Commands::COMMAND_ATTACK;
    m.move_num = 2;

    m.pos = PLAYER_1_0;
    m.target_pos = PLAYER_2_0;
    BSM.turn_messages.push_back(m);
    m.pos = PLAYER_1_1;
    m.target_pos = PLAYER_2_1;
    BSM.turn_messages.push_back(m);

    m.move_command = Commands::COMMAND_SWAP;

    m.pos = PLAYER_2_0;
    m.reserve_poke = 1;
    BSM.turn_messages.push_back(m);

    m.move_command = Commands::COMMAND_ATTACK;

    m.pos = PLAYER_2_1;
    m.target_pos = PLAYER_1_1;
    BSM.turn_messages.push_back(m);

    BSM.sort_message_stack();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_0);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_1);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_2_1);
    BSM.turn_messages.pop_back();
    EXPECT_EQ(BSM.turn_messages.back().pos, PLAYER_1_0);
    BSM.turn_messages.pop_back();
}