//
// Created by slist on 10/31/19.
//

#include <gtest/gtest.h>
#include <Pokemon/Natures.h>
#include <map>

map<Natures, vector<float>> nature_stat_mod = {
        {Natures::HARDY,   {1, 1, 1, 1, 1, 1}},
        {Natures::LONELY,  {1, 1.1, 0.9, 1, 1, 1}},
        {Natures::BRAVE,   {1, 1.1, 1, 1, 1, 0.9}},
        {Natures::ADAMANT, {1, 1.1, 1, 0.9, 1, 1}},
        {Natures::NAUGHTY, {1, 1.1, 1, 1, 0.9, 1}},
        {Natures::BOLD,    {1, 0.9, 1.1, 1, 1, 1}},
        {Natures::DOCILE,  {1, 1, 1, 1, 1, 1}},
        {Natures::RELAXED, {1, 1, 1.1, 1, 1, 0.9}},
        {Natures::IMPISH,  {1, 1, 1.1, 0.9, 1, 1}},
        {Natures::LAX,     {1, 1, 1.1, 1, 0.9, 1}},
        {Natures::TIMID,   {1, 0.9, 1, 1, 1, 1.1}},
        {Natures::HASTY,   {1, 1, 0.9, 1, 1, 1.1}},
        {Natures::SERIOUS, {1, 1, 1, 1, 1, 1}},
        {Natures::JOLLY,   {1, 1, 1, 0.9, 1, 1.1}},
        {Natures::NAIVE,   {1, 1, 1, 1, 0.9, 1.1}},
        {Natures::MODEST,  {1, 0.9, 1, 1.1, 1, 1}},
        {Natures::MILD,    {1, 1, 0.9, 1.1, 1, 1}},
        {Natures::QUIET,   {1, 1, 1, 1.1, 1, 0.9}},
        {Natures::BASHFUL, {1, 1, 1, 1, 1, 1}},
        {Natures::RASH,    {1, 1, 1, 1.1, 0.9, 1}},
        {Natures::CALM,    {1, 0.9, 1, 1, 1.1, 1}},
        {Natures::GENTLE,  {1, 1, 0.9, 1, 1.1, 1}},
        {Natures::SASSY,   {1, 1, 1, 1, 1.1, 0.9}},
        {Natures::CAREFUL, {1, 1, 1, 0.9, 1.1, 1}},
        {Natures::QUIRKY,  {1, 1, 1, 1, 1, 1}}
};

TEST(test_string_to_nature, all)
{
    EXPECT_EQ(string_to_nature("HARDY"), Natures::HARDY);
    EXPECT_EQ(string_to_nature("LONELY"), Natures::LONELY);
    EXPECT_EQ(string_to_nature("BRAVE"), Natures::BRAVE);
    EXPECT_EQ(string_to_nature("ADAMANT"), Natures::ADAMANT);
    EXPECT_EQ(string_to_nature("NAUGHTY"), Natures::NAUGHTY);
    EXPECT_EQ(string_to_nature("BOLD"), Natures::BOLD);
    EXPECT_EQ(string_to_nature("DOCILE"), Natures::DOCILE);
    EXPECT_EQ(string_to_nature("RELAXED"), Natures::RELAXED);
    EXPECT_EQ(string_to_nature("IMPISH"), Natures::IMPISH);
    EXPECT_EQ(string_to_nature("LAX"), Natures::LAX);
    EXPECT_EQ(string_to_nature("TIMID"), Natures::TIMID);
    EXPECT_EQ(string_to_nature("HASTY"), Natures::HASTY);
    EXPECT_EQ(string_to_nature("SERIOUS"), Natures::SERIOUS);
    EXPECT_EQ(string_to_nature("JOLLY"), Natures::JOLLY);
    EXPECT_EQ(string_to_nature("NAIVE"), Natures::NAIVE);
    EXPECT_EQ(string_to_nature("MODEST"), Natures::MODEST);
    EXPECT_EQ(string_to_nature("MILD"), Natures::MILD);
    EXPECT_EQ(string_to_nature("QUIET"), Natures::QUIET);
    EXPECT_EQ(string_to_nature("BASHFUL"), Natures::BASHFUL);
    EXPECT_EQ(string_to_nature("RASH"), Natures::RASH);
    EXPECT_EQ(string_to_nature("CALM"), Natures::CALM);
    EXPECT_EQ(string_to_nature("GENTLE"), Natures::GENTLE);
    EXPECT_EQ(string_to_nature("SASSY"), Natures::SASSY);
    EXPECT_EQ(string_to_nature("CAREFUL"), Natures::CAREFUL);
    EXPECT_EQ(string_to_nature("QUIRKY"), Natures::QUIRKY);
}

TEST(test_get_nature_mod, ALL)
{
    for(int i = Natures::HARDY; i < Natures::NUM_NATURES; i++)
        for(int j = STAT::ATK; j <= STAT::SPE; j++)
             EXPECT_EQ(get_nature_mod(static_cast<Natures>(i), static_cast<STAT>(j)) ,nature_stat_mod[static_cast<Natures>(i)][j]);

}