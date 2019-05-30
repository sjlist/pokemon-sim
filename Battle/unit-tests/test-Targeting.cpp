//
// Created by slist on 5/22/19.
//

#include <gtest/gtest.h>
#include <Battle/Players.h>
#include <Battle/FieldPositions.h>
#include <Battle/Targeting.h>

TEST(test_is_adjacent, ADJACENT)
{
    Targets t;
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_0, PLAYER_2_0));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_0, PLAYER_1_0));
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_0, PLAYER_1_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_0, PLAYER_2_1));

    EXPECT_TRUE(t.is_adjacent(PLAYER_1_1, PLAYER_1_0));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_1, PLAYER_2_0));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_1, PLAYER_2_1));

    EXPECT_TRUE(t.is_adjacent(PLAYER_2_0, PLAYER_1_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_0, PLAYER_2_1));

    EXPECT_TRUE(t.is_adjacent(PLAYER_2_1, PLAYER_1_0));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_1, PLAYER_1_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_1, PLAYER_2_0));
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_1, PLAYER_1_2));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_1, PLAYER_2_2));

    EXPECT_TRUE(t.is_adjacent(PLAYER_1_2, PLAYER_1_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_2, PLAYER_2_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_1_2, PLAYER_2_2));

    EXPECT_TRUE(t.is_adjacent(PLAYER_2_1, PLAYER_1_2));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_1, PLAYER_2_2));

    EXPECT_TRUE(t.is_adjacent(PLAYER_2_2, PLAYER_1_1));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_2, PLAYER_1_2));
    EXPECT_TRUE(t.is_adjacent(PLAYER_2_2, PLAYER_2_1));
#endif
}

TEST(test_is_adjacent, NOT_ADJACENT)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_0, PLAYER_1_2));
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_0, PLAYER_2_2));

    EXPECT_FALSE(t.is_adjacent(PLAYER_1_2, PLAYER_1_0));
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_2, PLAYER_2_0));

    EXPECT_FALSE(t.is_adjacent(PLAYER_2_0, PLAYER_1_2));
    EXPECT_FALSE(t.is_adjacent(PLAYER_2_0, PLAYER_2_2));

    EXPECT_FALSE(t.is_adjacent(PLAYER_2_2, PLAYER_1_0));
    EXPECT_FALSE(t.is_adjacent(PLAYER_2_2, PLAYER_2_0));
#endif
}


TEST(test_is_adjacent, SAME)
{
    Targets t;
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_0, PLAYER_1_0));
    EXPECT_FALSE(t.is_adjacent(PLAYER_2_0, PLAYER_2_0));
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_1, PLAYER_1_1));
    EXPECT_FALSE(t.is_adjacent(PLAYER_2_1, PLAYER_2_1));
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_FALSE(t.is_adjacent(PLAYER_1_2, PLAYER_1_2));
    EXPECT_FALSE(t.is_adjacent(PLAYER_2_2, PLAYER_2_2));
#endif
}

TEST(test_get_relative_position, happy)
{
    Targets t;
    EXPECT_EQ(t.get_relative_position(PLAYER_1_0), 0);
    EXPECT_EQ(t.get_relative_position(PLAYER_2_0), 0);
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.get_relative_position(PLAYER_1_1), 1);
    EXPECT_EQ(t.get_relative_position(PLAYER_2_1), 1);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.get_relative_position(PLAYER_1_2), 2);
    EXPECT_EQ(t.get_relative_position(PLAYER_2_2), 2);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ALL_1_0)
{
    Targets t;
    t.get_valid_targets(ADJACENT_ALL, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ALL_1_1)
{
    Targets t;
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_ALL, PLAYER_1_1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 5);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ALL_1_2)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_ALL, PLAYER_1_2);

    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ENEMY_1_0)
{
    Targets t;
    t.get_valid_targets(ADJACENT_ENEMY, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ENEMY_1_1)
{
    Targets t;
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_ENEMY, PLAYER_1_1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ADJACENT_ENEMY_1_2)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_ENEMY, PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
}

TEST(test_get_valid_targets, ADJACENT_FRIENDLY_1_0)
{
    Targets t;
    t.get_valid_targets(ADJACENT_FRIENDLY, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.get_num_valid_targets(), 0);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
}

TEST(test_get_valid_targets, ADJACENT_FRIENDLY_1_1)
{
    Targets t;
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_FRIENDLY, PLAYER_1_1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_2);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
}

TEST(test_get_valid_targets, ADJACENT_FRIENDLY_1_2)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ADJACENT_FRIENDLY, PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
}

TEST(test_get_valid_targets, ALL_ALL_1_0)
{
    Targets t;
    t.get_valid_targets(ALL_ALL, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 4);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[5], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 6);
#endif
}
TEST(test_get_valid_targets, ALL_ALL_2_0)
{
    Targets t;
    t.get_valid_targets(ALL_ALL, PLAYER_2_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 4);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[5], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 6);
#endif
}

TEST(test_get_valid_targets, ALL_OTHERS_1_0)
{
    Targets t;
    t.get_valid_targets(ALL_OTHERS, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 5);
#endif
}

TEST(test_get_valid_targets, ALL_OTHERS_1_1)
{
    Targets t;
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ALL_OTHERS, PLAYER_1_1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 5);
#endif
}

TEST(test_get_valid_targets, ALL_OTHERS_1_2)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ALL_OTHERS, PLAYER_1_2);
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[3], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[4], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 5);
#endif
}

TEST(test_get_valid_targets, ALL_ENEMY_1_0)
{
    Targets t;
    t.get_valid_targets(ALL_ENEMY, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ALL_ENEMY_1_2)
{
    Targets t;
#if BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ALL_ENEMY, PLAYER_1_2);

    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ALL_ENEMY_2_0)
{
    Targets t;
    t.get_valid_targets(ALL_ENEMY, PLAYER_2_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_1_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ALL_FRIENDLY_1_0)
{
    Targets t;
    t.get_valid_targets(ALL_FRIENDLY, PLAYER_1_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_1_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ALL_FRIENDLY_1_1)
{
    Targets t;
#if BATTLE_TYPE == DOUBLE_BATTLE || BATTLE_TYPE == TRIPLE_BATTLE
    t.get_valid_targets(ALL_FRIENDLY, PLAYER_1_1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_1_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_1_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, ALL_FRIENDLY_2_0)
{
    Targets t;
    t.get_valid_targets(ALL_FRIENDLY, PLAYER_2_0);
#if BATTLE_TYPE == SINGLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.get_num_valid_targets(), 2);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[1], PLAYER_2_1);
    EXPECT_EQ(t.valid_targets[2], PLAYER_2_2);
    EXPECT_EQ(t.get_num_valid_targets(), 3);
#endif
}

TEST(test_get_valid_targets, SELF)
{
    Targets t;
    t.get_valid_targets(SELF, PLAYER_2_0);
    EXPECT_EQ(t.valid_targets[0], PLAYER_2_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);

    t.get_valid_targets(SELF, PLAYER_1_0);
    EXPECT_EQ(t.valid_targets[0], PLAYER_1_0);
    EXPECT_EQ(t.get_num_valid_targets(), 1);
}