//
// Created by slist on 5/22/19.
//

#include <gtest/gtest.h>
#include <Battle/BattleActor.h>

class ActorTest: public BattleActor
{
private:
    FRIEND_TEST(make_pokemon_choice, 6);
    FRIEND_TEST(make_pokemon_choice, 5);
    FRIEND_TEST(make_pokemon_choice, 4);
    FRIEND_TEST(make_pokemon_choice, 3);
    FRIEND_TEST(make_pokemon_choice, 2);
    FRIEND_TEST(make_pokemon_choice, 1);
    FRIEND_TEST(roll_chance, 100);
    FRIEND_TEST(roll_chance, 90);
    FRIEND_TEST(roll_chance, 50);
    FRIEND_TEST(roll_chance, 10);
};

TEST(make_pokemon_choice, 6)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 6;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(make_pokemon_choice, 5)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 5;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(make_pokemon_choice, 4)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 4;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(make_pokemon_choice, 3)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 3;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(make_pokemon_choice, 2)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 2;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(make_pokemon_choice, 1)
{
    ActorTest a;
    int num_runs = 1000000, num_positions = 1;
    float res[6] = {0, 0, 0, 0, 0, 0};
    random_device rd;
    long seed = rd();

    a.update_generator(seed);

    for(int i = 0; i < num_runs; i++)
    {
        res[a.choose_position(num_positions)]++;
    }

    for(int i = 0; i < num_positions; i++)
    {
        res[i] = res[i] / num_runs * 100;
        EXPECT_NEAR(res[i], 100.0/num_positions, 0.2);
    }
}

TEST(roll_chance, 100)
{
    ActorTest a;
    int num_runs = 100000;
    float res[2] = {0, 0}, pcent_chance = 100;

    for(int i = 0; i < num_runs; i++)
    {
        if(a.roll_chance(pcent_chance/100))
            res[1]++;
        else
            res[0]++;
    }


    res[0] = res[0] / num_runs * 100;
    res[1] = res[1] / num_runs * 100;

    EXPECT_NEAR(res[0], (100-pcent_chance), 0.2);
    EXPECT_NEAR(res[1], pcent_chance, 0.2);
}

TEST(roll_chance, 90)
{
    ActorTest a;
    int num_runs = 100000;
    float res[2] = {0, 0}, pcent_chance = 90;

    for(int i = 0; i < num_runs; i++)
    {
        if(a.roll_chance(pcent_chance/100))
            res[1]++;
        else
            res[0]++;
    }


    res[0] = res[0] / num_runs * 100;
    res[1] = res[1] / num_runs * 100;

    EXPECT_NEAR(res[0], (100-pcent_chance), 0.2);
    EXPECT_NEAR(res[1], pcent_chance, 0.2);
}

TEST(roll_chance, 50)
{
    ActorTest a;
    int num_runs = 100000;
    float res[2] = {0, 0}, pcent_chance = 50;

    for(int i = 0; i < num_runs; i++)
    {
        if(a.roll_chance(pcent_chance/100))
            res[1]++;
        else
            res[0]++;
    }


    res[0] = res[0] / num_runs * 100;
    res[1] = res[1] / num_runs * 100;

    EXPECT_NEAR(res[0], (100-pcent_chance), 0.2);
    EXPECT_NEAR(res[1], pcent_chance, 0.2);
}

TEST(roll_chance, 10)
{
    ActorTest a;
    int num_runs = 100000;
    float res[2] = {0, 0}, pcent_chance = 10;

    for(int i = 0; i < num_runs; i++)
    {
        if(a.roll_chance(pcent_chance/100))
            res[1]++;
        else
            res[0]++;
    }


    res[0] = res[0] / num_runs * 100;
    res[1] = res[1] / num_runs * 100;

    EXPECT_NEAR(res[0], (100-pcent_chance), 0.2);
    EXPECT_NEAR(res[1], pcent_chance, 0.2);
}