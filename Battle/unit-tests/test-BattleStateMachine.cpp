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
    void load_test_teams(string team1, string team2, vector<int>* send_outs);

private:
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_same_prio);
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks);
    FRIEND_TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks_swap);
    FRIEND_TEST(test_sort_message_stack, 1_2_3_4_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 1_2_3_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 2_3_4_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 1_2_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 2_3_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 3_4_speed_tie);
    FRIEND_TEST(test_sort_message_stack, 1_2_and_3_4_speed_tie);
};

void BSMTest::load_test_teams(string team1, string team2, vector<int>* send_outs)
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

    for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
    {
        m.pos = static_cast<FIELD_POSITION>(i);
        m.reserve_poke = send_outs->at(i);
        BSMTest::run(m);
    }
    EXPECT_EQ(BSMTest::state, BattleState::ACTION_REQUEST);
}

TEST(test_sort_message_stack, no_speed_ties_same_prio) {
    //Team 2 has 0 speed IVs team 1 has 31. All else held equal. So in priority ties between pokes os same species, team1 wins
    BSMTest BSM;
    BattleMessage m;
    vector<int> send_outs = {0, 2, 0, 2};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs);

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
    vector<int> send_outs = {0, 2, 0, 2};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs);

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
    vector<int> send_outs = {0, 2, 0, 2};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs);

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

TEST(test_sort_message_stack, 1_2_3_4_speed_tie)
{
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {25, 25, 25, 25};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, tie_pcents};
    vector<int> send_outs = {0, 1, 0, 1};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 1_2_3_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    vector<int> send_outs = {0, 1, 0, 4};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 2_3_4_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, tie_pcents};
    vector<int> send_outs = {0, 2, 2, 3};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 1_2_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, 0, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, {0, 0, 100, 0}, tie_pcents, {0, 0, 0, 100}};
    vector<int> send_outs = {0, 2, 0, 4};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 2_3_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    vector<int> send_outs = {0, 2, 2, 4};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, {0, 100, 0, 0}, tie_pcents, tie_pcents};
    vector<int> send_outs = {0, 2, 4, 5};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}

TEST(test_sort_message_stack, 1_2_and_3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> order_locs (4, vector<float>(4));
    vector<float> tie_pcents1 = {probabilty, probabilty, 0, 0};
    vector<float> tie_pcents2 = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {tie_pcents1, tie_pcents2, tie_pcents1, tie_pcents2};
    vector<int> send_outs = {0, 4, 0, 4};
    int num_sorts = 200000;
    float error_allowed = 0.5;
    BSMTest BSM;
    BattleMessage m;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs);

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

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack();
        order_locs[BSM.turn_messages[3].pos][0]++;
        order_locs[BSM.turn_messages[2].pos][1]++;
        order_locs[BSM.turn_messages[1].pos][2]++;
        order_locs[BSM.turn_messages[0].pos][3]++;
    }

    for (int i = 0; i < order_locs.size(); i++)
    {
        for (int j = 0; j < order_locs[i].size(); j++)
        {
            order_locs[i][j] = order_locs[i][j]/num_sorts*100;
            if(expected_pcent[i][j] == 0)
                EXPECT_EQ(expected_pcent[i][j], order_locs[i][j]);
            else
                EXPECT_NEAR(order_locs[i][j], expected_pcent[i][j], error_allowed);
        }
    }
}