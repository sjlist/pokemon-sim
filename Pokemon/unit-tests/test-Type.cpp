//
// Created by slist on 10/29/19.
//

#include <gtest/gtest.h>
#include <Pokemon/Type.h>

TEST(test_string_to_type, happy)
{
    EXPECT_EQ(string_to_type("NORMAL"), PokeTypes::NORMAL);
    EXPECT_EQ(string_to_type("FIRE"), PokeTypes::FIRE);
    EXPECT_EQ(string_to_type("WATER"), PokeTypes::WATER);
    EXPECT_EQ(string_to_type("GRASS"), PokeTypes::GRASS);
    EXPECT_EQ(string_to_type("ELECTRIC"), PokeTypes::ELECTRIC);
    EXPECT_EQ(string_to_type("ICE"), PokeTypes::ICE);
    EXPECT_EQ(string_to_type("FIGHTING"), PokeTypes::FIGHTING);
    EXPECT_EQ(string_to_type("POISON"), PokeTypes::POISON);
    EXPECT_EQ(string_to_type("GROUND"), PokeTypes::GROUND);
    EXPECT_EQ(string_to_type("FLYING"), PokeTypes::FLYING);
    EXPECT_EQ(string_to_type("PSYCHIC"), PokeTypes::PSYCHIC);
    EXPECT_EQ(string_to_type("BUG"), PokeTypes::BUG);
    EXPECT_EQ(string_to_type("ROCK"), PokeTypes::ROCK);
    EXPECT_EQ(string_to_type("GHOST"), PokeTypes::GHOST);
    EXPECT_EQ(string_to_type("DRAGON"), PokeTypes::DRAGON);
    EXPECT_EQ(string_to_type("DARK"), PokeTypes::DARK);
    EXPECT_EQ(string_to_type("STEEL"), PokeTypes::STEEL);
    EXPECT_EQ(string_to_type("FAIRY"), PokeTypes::FAIRY);
    EXPECT_EQ(string_to_type("NONE"), PokeTypes::NO_TYPE);
}

TEST(test_type_to_string, happy)
{
    EXPECT_EQ(type_to_string(PokeTypes::NORMAL), "NORMAL");
    EXPECT_EQ(type_to_string(PokeTypes::FIRE), "FIRE");
    EXPECT_EQ(type_to_string(PokeTypes::WATER), "WATER");
    EXPECT_EQ(type_to_string(PokeTypes::GRASS), "GRASS");
    EXPECT_EQ(type_to_string(PokeTypes::ELECTRIC), "ELECTRIC");
    EXPECT_EQ(type_to_string(PokeTypes::ICE), "ICE");
    EXPECT_EQ(type_to_string(PokeTypes::FIGHTING), "FIGHTING");
    EXPECT_EQ(type_to_string(PokeTypes::POISON), "POISON");
    EXPECT_EQ(type_to_string(PokeTypes::GROUND), "GROUND");
    EXPECT_EQ(type_to_string(PokeTypes::FLYING), "FLYING");
    EXPECT_EQ(type_to_string(PokeTypes::PSYCHIC), "PSYCHIC");
    EXPECT_EQ(type_to_string(PokeTypes::BUG), "BUG");
    EXPECT_EQ(type_to_string(PokeTypes::ROCK), "ROCK");
    EXPECT_EQ(type_to_string(PokeTypes::GHOST), "GHOST");
    EXPECT_EQ(type_to_string(PokeTypes::DRAGON), "DRAGON");
    EXPECT_EQ(type_to_string(PokeTypes::DARK), "DARK");
    EXPECT_EQ(type_to_string(PokeTypes::STEEL), "STEEL");
    EXPECT_EQ(type_to_string(PokeTypes::FAIRY), "FAIRY");
    EXPECT_EQ(type_to_string(PokeTypes::NO_TYPE), "NONE");
}

TEST(test_is_stab, first_stab)
{
    PokeTypes type[2] = {PokeTypes::DARK, PokeTypes::FIRE};
    EXPECT_TRUE(is_stab(type, PokeTypes::DARK));
}

TEST(test_is_stab, second_stab)
{
    PokeTypes type[2] = {PokeTypes::DARK, PokeTypes::FIRE};
    EXPECT_TRUE(is_stab(type, PokeTypes::FIRE));
}

TEST(test_is_stab, move_no_type)
{
    PokeTypes type[2] = {PokeTypes::DARK, PokeTypes::FIRE};
    EXPECT_FALSE(is_stab(type, PokeTypes::NO_TYPE));
}

TEST(test_is_stab, move_no_type_poke_one_type)
{
    PokeTypes type[2] = {PokeTypes::DARK, PokeTypes::NO_TYPE};
    EXPECT_FALSE(is_stab(type, PokeTypes::NO_TYPE));
}

TEST(test_calculate_type_damage_modifier, ST)
{
    PokeTypes type[2] = {PokeTypes::FLYING, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::NORMAL), ST_DM);
}

TEST(test_calculate_type_damage_modifier, SE)
{
    PokeTypes type[2] = {PokeTypes::FLYING, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::WATER), SE_DM);
}

TEST(test_calculate_type_damage_modifier, SE_SE)
{
    PokeTypes type[2] = {PokeTypes::FLYING, PokeTypes::WATER};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::ELECTRIC), SE_DM*SE_DM);
}

TEST(test_calculate_type_damage_modifier, NE)
{
    PokeTypes type[2] = {PokeTypes::FLYING, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::FIGHTING), NE_DM);
}

TEST(test_calculate_type_damage_modifier, NE_NE)
{
    PokeTypes type[2] = {PokeTypes::FLYING, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::GRASS), NE_DM*NE_DM);
}

TEST(test_calculate_type_damage_modifier, SE_NE)
{
    PokeTypes type[2] = {PokeTypes::GRASS, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::WATER), ST_DM);
}

TEST(test_calculate_type_damage_modifier, IM_NE)
{
    PokeTypes type[2] = {PokeTypes::FAIRY, PokeTypes::STEEL};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::DRAGON), IM_DM);
}

TEST(test_calculate_type_damage_modifier, IM_ST)
{
    PokeTypes type[2] = {PokeTypes::FAIRY, PokeTypes::FIRE};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::DRAGON), IM_DM);
}

TEST(test_calculate_type_damage_modifier, IM_EE)
{
    PokeTypes type[2] = {PokeTypes::FAIRY, PokeTypes::DRAGON};
    EXPECT_EQ(calculate_type_damage_modifier(type, PokeTypes::DRAGON), IM_DM);
}

TEST(test_calculate_damage_modifier_single, NORMAL)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::FIGHTING), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::GHOST),    IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NORMAL, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, FIRE)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::FIRE),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::WATER),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::ICE),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::GROUND),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::ROCK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::STEEL),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::FAIRY),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIRE, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, WATER)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::FIRE),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::WATER),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::GRASS),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::ELECTRIC), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::ICE),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::STEEL),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::WATER, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, GRASS)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::FIRE),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::WATER),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::ELECTRIC), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::ICE),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::POISON),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::GROUND),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::FLYING),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::BUG),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GRASS, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, ELECTRIC)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::ELECTRIC), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::GROUND),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::FLYING),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ELECTRIC, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, ICE)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::FIRE),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::ICE),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::FIGHTING), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::ROCK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::STEEL),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ICE, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, FIGHTING)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::FLYING),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::PSYCHIC),  SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::ROCK),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::DARK),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::FAIRY),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FIGHTING, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, POISON)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::FIGHTING), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::POISON),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::GROUND),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::PSYCHIC),  SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::FAIRY),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::POISON, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, FLYING)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::ELECTRIC), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::ICE),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::FIGHTING), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::GROUND),   IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::ROCK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FLYING, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, PSYCHIC)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::FIGHTING), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::PSYCHIC),  NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::BUG),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::GHOST),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::DARK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::PSYCHIC, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, BUG)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::FIRE),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::FIGHTING), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::GROUND),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::FLYING),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::ROCK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::BUG, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, ROCK)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::NORMAL),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::FIRE),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::WATER),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::GRASS),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::FIGHTING), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::POISON),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::GROUND),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::FLYING),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::STEEL),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::ROCK, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, GHOST)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::NORMAL),   IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::FIGHTING), IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::POISON),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::GHOST),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::DARK),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::GHOST, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, DRAGON)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::FIRE),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::WATER),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::ELECTRIC), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::ICE),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::DRAGON),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::FAIRY),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DRAGON, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, DARK)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::FIGHTING), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::PSYCHIC),  IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::BUG),      SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::GHOST),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::DARK),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::FAIRY),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::DARK, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, STEEL)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::NORMAL),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::FIRE),     SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::GRASS),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::ICE),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::FIGHTING), SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::POISON),   IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::GROUND),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::FLYING),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::PSYCHIC),  NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::ROCK),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::DRAGON),   NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::STEEL),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::FAIRY),    NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::STEEL, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, FAIRY)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::FIGHTING), NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::POISON),   SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::BUG),      NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::DRAGON),   IM_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::DARK),     NE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::STEEL),    SE_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::FAIRY, PokeTypes::NO_TYPE),  ST_DM);
}

TEST(test_calculate_damage_modifier_single, NO_TYPE)
{
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::NORMAL),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::FIRE),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::WATER),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::GRASS),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::ELECTRIC), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::ICE),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::FIGHTING), ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::POISON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::GROUND),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::FLYING),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::PSYCHIC),  ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::BUG),      ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::ROCK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::GHOST),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::DRAGON),   ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::DARK),     ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::STEEL),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::FAIRY),    ST_DM);
    EXPECT_EQ(calculate_damage_modifier_single(PokeTypes::NO_TYPE, PokeTypes::NO_TYPE),  ST_DM);
}