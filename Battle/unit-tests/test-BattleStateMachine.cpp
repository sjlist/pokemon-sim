//
// Created by slist on 6/27/19.
//

#include <gtest/gtest.h>
#include <Battle/BattleStateMachine.h>
#include <Battle/BattleMessage.h>

#include "probability-helpers.h"

using namespace std;

class BSMTest: public BattleStateMachine
{
public:
    void load_test_teams(string team1, string team2, vector<int>* send_outs, vector<int>* moves);

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

    FRIEND_TEST(test_create_speed_list, no_ties);
    FRIEND_TEST(test_create_speed_list, 1_2_3_4_speed_tie);
    FRIEND_TEST(test_create_speed_list, 1_2_3_speed_tie);
    FRIEND_TEST(test_create_speed_list, 2_3_4_speed_tie);
    FRIEND_TEST(test_create_speed_list, 1_2_speed_tie);
    FRIEND_TEST(test_create_speed_list, 2_3_speed_tie);
    FRIEND_TEST(test_create_speed_list, 3_4_speed_tie);
    FRIEND_TEST(test_create_speed_list, 1_2_and_3_4_speed_tie);
};

void BSMTest::load_test_teams(string team1, string team2, vector<int>* send_outs, vector<int>* moves)
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

    for(unsigned int i = 0; i < moves->size(); i++)
    {
        if(moves->at(i) == 5)
        {
            m.move_command = Commands::COMMAND_SWAP;
        }
        else
        {
            m.move_command = Commands::COMMAND_ATTACK;
            m.move_num = moves->at(i);
        }
        m.pos = static_cast<FIELD_POSITION>(i);
        BSMTest::turn_messages.push_back(m);
    }
}

void check_order(vector<BattleMessage>* turn_messages, vector<int>* expected)
{
    for(unsigned int i = 0; i < expected->size(); i++)
        EXPECT_EQ(static_cast<FIELD_POSITION>(i), turn_messages->at(expected->at(i)).pos);
}

TEST(test_sort_message_stack, no_speed_ties_same_prio) {
    //Team 2 has 0 speed IVs team 1 has 31. All else held equal. So in priority ties between pokes os same species, team1 wins
    BSMTest BSM;
    vector<int> send_outs = {0, 2, 0, 2};
    vector<int> moves     = {0, 0, 0, 0};
    vector<int> expected  = {3, 1, 2, 0};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    BSM.sort_message_stack(&BSM.turn_messages);

    check_order(&BSM.turn_messages, &expected);
}

TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks)
{
    BSMTest BSM;
    vector<int> send_outs = {0, 2, 0, 2};
    vector<int> moves     = {2, 2, 2, 2};
    vector<int> expected  = {1, 3, 0, 2};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    BSM.sort_message_stack(&BSM.turn_messages);

    check_order(&BSM.turn_messages, &expected);
}

TEST(test_sort_message_stack, no_speed_ties_diff_prio_attacks_swap) {
    BSMTest BSM;
    vector<int> send_outs = {0, 2, 0, 2};
    vector<int> moves = {2, 2, 5, 2};
    vector<int> expected = {0, 2, 3, 1};
    string team1 = "unit-test/test-team1";
    string team2 = "unit-test/test-team2";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    BSM.sort_message_stack(&BSM.turn_messages);

    check_order(&BSM.turn_messages, &expected);
}

void test_probability(vector<vector<float>>* diff)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            EXPECT_LE(abs(diff->at(i)[j]), THRESHOLD*100);
        }
    }
}

TEST(test_sort_message_stack, 1_2_3_4_speed_tie)
{
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {25, 25, 25, 25};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(16);

    vector<int> send_outs = {0, 1, 0, 1};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 1_2_3_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 1, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 2_3_4_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 2, 2, 3};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);


    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 1_2_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, 0, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, {0, 0, 100, 0}, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 2_3_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 2, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, {0, 100, 0, 0}, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 4, 5};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_sort_message_stack, 1_2_and_3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents1 = {probabilty, probabilty, 0, 0};
    vector<float> tie_pcents2 = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {tie_pcents1, tie_pcents2, tie_pcents1, tie_pcents2};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 4, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.sort_message_stack(&BSM.turn_messages);
        pcent_deviation[BSM.turn_messages[3].pos][0]++;
        pcent_deviation[BSM.turn_messages[2].pos][1]++;
        pcent_deviation[BSM.turn_messages[1].pos][2]++;
        pcent_deviation[BSM.turn_messages[0].pos][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 1_2_3_4_speed_tie)
{
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {25, 25, 25, 25};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(16);

    vector<int> send_outs = {0, 1, 0, 1};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 1_2_3_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 1, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 2_3_4_speed_tie)
{
    float probabilty = 33.333333;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 2, 2, 3};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 1_2_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {probabilty, probabilty, 0, 0};
    vector<vector<float>> expected_pcent = {tie_pcents, {0, 0, 100, 0}, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 2_3_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, probabilty, probabilty, 0};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, tie_pcents, tie_pcents, {0, 0, 0, 100}};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 2, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {{100, 0, 0, 0}, {0, 100, 0, 0}, tie_pcents, tie_pcents};
    int num_sorts = get_num_samples(4);

    vector<int> send_outs = {0, 2, 4, 5};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation[BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}

TEST(test_create_speed_list, 1_2_and_3_4_speed_tie)
{
    float probabilty = 50;
    vector<vector<float>> pcent_deviation (4, vector<float>(4));
    vector<float> tie_pcents1 = {probabilty, probabilty, 0, 0};
    vector<float> tie_pcents2 = {0, 0, probabilty, probabilty};
    vector<vector<float>> expected_pcent = {tie_pcents1, tie_pcents2, tie_pcents1, tie_pcents2};
    int num_sorts = get_num_samples(8);

    vector<int> send_outs = {0, 4, 0, 4};
    vector<int> moves     = {0, 0, 0, 0};

    BSMTest BSM;
    string team1 = "unit-test/test-team-speed-tie";
    string team2 = "unit-test/test-team-speed-tie";

    BSM.load_test_teams(team1, team2, &send_outs, &moves);

    for(int i = 0; i < num_sorts; i++)
    {
        BSM.create_speed_list();
        pcent_deviation[BSM.speed_list[0]][0]++;
        pcent_deviation[BSM.speed_list[1]][1]++;
        pcent_deviation[BSM.speed_list[2]][2]++;
        pcent_deviation [BSM.speed_list[3]][3]++;
    }

    // Turn pcent_deviation into the deviation from expected
    for (unsigned int i = 0; i < pcent_deviation.size(); i++)
    {
        for (unsigned int j = 0; j < pcent_deviation[i].size(); j++)
        {
            pcent_deviation[i][j] = pcent_deviation[i][j]/num_sorts*100 - expected_pcent[i][j];
        }
    }

    test_probability(&pcent_deviation);
}