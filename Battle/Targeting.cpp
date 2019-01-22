//
// Created by slist on 1/3/19.
//

#include <Battle/Field.h>
#include <Battle/Targeting.h>

#include <map>

static std::map<std::string, TARGETS> string_target_map = {
        { "ADJACENT_ALL", TARGETS::ADJACENT_ALL},
        { "ADJACENT_ENEMY", TARGETS::ADJACENT_ENEMY },
        { "ADJACENT_FRIENDLY", TARGETS::ADJACENT_FRIENDLY},
        { "ALL_ALL", TARGETS::ALL_ALL},
        { "ALL_OTHERS", TARGETS::ALL_OTHERS},
        { "ALL_ENEMY", TARGETS::ALL_ENEMY },
        { "ALL_FRIENDLY", TARGETS::ALL_FRIENDLY},
        { "SELF", TARGETS::SELF}
};

Targets::Targets()
{}

void Targets::get_valid_targets(TARGETS attack_target, FIELD_POSITION atk_pos)
{
    int friendly_offset = get_player_from_position(atk_pos)*FIELD_POSITION::NUM_POSITIONS/2;
    int enemy_offset = FIELD_POSITION::NUM_POSITIONS/2 - friendly_offset;
    switch(attack_target)
    {
#if BATTLE_TYPE == SINGLE_BATTLE
        case ADJACENT_ALL:
        case ADJACENT_ENEMY:
        case ALL_OTHERS:
        case ALL_ENEMY:
            if(get_player_from_position(atk_pos) == PLAYER_ONE)
                Targets::valid_targets[0] = FIELD_POSITION::PLAYER_2_0;
            else
                Targets::valid_targets[0] = FIELD_POSITION::PLAYER_1_0;
            Targets::num_valid_targets = 1;
            break;;
        case ADJACENT_FRIENDLY:
        case ALL_FRIENDLY:
            Targets::num_valid_targets = 0;
            break;
        case SELF:
            Targets::valid_targets[0] = atk_pos;
            Targets::num_valid_targets = 1;
            break;;
        case ALL_ALL:
            Targets::valid_targets[0] = FIELD_POSITION::PLAYER_1_0;
            Targets::valid_targets[1] = FIELD_POSITION::PLAYER_2_0;
            Targets::num_valid_targets = 2;
            break;;
#endif
#if BATTLE_TYPE == DOUBLE_BATTLE
        case ADJACENT_ALL:
        case ADJACENT_ENEMY:
        case ALL_OTHERS:
        case ALL_ENEMY:
        case ADJACENT_FRIENDLY:
        case ALL_FRIENDLY:
        case SELF:
        case ALL_ALL:
            assert(0);
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
        case ADJACENT_ALL:
        case ADJACENT_ENEMY:
        case ALL_OTHERS:
        case ALL_ENEMY:
        case ADJACENT_FRIENDLY:
        case ALL_FRIENDLY:
        case SELF:
        case ALL_ALL:
            assert(0);
#endif
        default:
            assert(0);
    }
}

int Targets::get_num_valid_targets()
{
    return Targets::num_valid_targets;
}

FIELD_POSITION Targets::get_single_target()
{
    return Targets::single_target;
}

void Targets::set_single_target(FIELD_POSITION pos)
{
    Targets::single_target = pos;
}

TARGETS string_to_target(std::string target_string)
{
    return string_target_map[target_string];
}