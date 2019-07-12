//
// Created by slist on 1/3/19.
//

#include <Battle/Field.h>
#include <Battle/Targeting.h>
#include <Utils/Logging.h>

#include <map>
using namespace std;

static map<string, TARGETS> string_target_map = {
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
    Targets::num_valid_targets = 0;
    switch(attack_target)
    {
        case ADJACENT_ALL:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(Targets::is_adjacent(atk_pos,static_cast<FIELD_POSITION>(i)))
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case ADJACENT_ENEMY:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(Targets::is_adjacent(atk_pos,static_cast<FIELD_POSITION>(i))
                && (get_player_from_position(static_cast<FIELD_POSITION>(i)) != get_player_from_position(atk_pos)))
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case ALL_OTHERS:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(static_cast<FIELD_POSITION>(i) != atk_pos)
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case ALL_ENEMY:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(get_player_from_position(static_cast<FIELD_POSITION>(i)) != get_player_from_position(atk_pos))
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case ADJACENT_FRIENDLY:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(Targets::is_adjacent(atk_pos,static_cast<FIELD_POSITION>(i))
                && (get_player_from_position(static_cast<FIELD_POSITION>(i)) == get_player_from_position(atk_pos)))
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case ALL_FRIENDLY:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {
                if(get_player_from_position(static_cast<FIELD_POSITION>(i)) == get_player_from_position(atk_pos))
                {
                    Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                    Targets::num_valid_targets++;
                }
            }
            break;
        case SELF:
            Targets::valid_targets[Targets::num_valid_targets] = atk_pos;
            Targets::num_valid_targets++;
            break;
        case ALL_ALL:
            for(int i = 0; i < FIELD_POSITION::NUM_POSITIONS; i++)
            {

                Targets::valid_targets[Targets::num_valid_targets] = static_cast<FIELD_POSITION>(i);
                Targets::num_valid_targets++;
            }
            break;
        default:
            ERR_MSG("Unhandled targeting case\n");
    }
}

int Targets::get_num_valid_targets()
{
    return Targets::num_valid_targets;
}

int Targets::get_relative_position(FIELD_POSITION pos)
{
    return (static_cast<int>(pos) % (static_cast<int>(FIELD_POSITION::NUM_POSITIONS)/2));
}

bool Targets::is_adjacent(FIELD_POSITION atk_pos, FIELD_POSITION def_pos)
{
    int relative_atk_pos = Targets::get_relative_position(atk_pos);
    int relative_def_pos = Targets::get_relative_position(def_pos);
    return (relative_def_pos >= (relative_atk_pos - 1))
         && (relative_def_pos <= (relative_atk_pos + 1))
         && atk_pos != def_pos;
}

FIELD_POSITION Targets::get_single_target()
{
    return Targets::single_target;
}

void Targets::set_single_target(FIELD_POSITION pos)
{
    Targets::single_target = pos;
}

TARGETS string_to_target(string target_string)
{
    return string_target_map[target_string];
}