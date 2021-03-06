//
// Created by slist on 1/3/19.
//

#include <Battle/FieldPositions.h>
#include <Config.h>
#include <Utils/Logging.h>
using namespace std;

string get_string_from_field_position(FIELD_POSITION pos)
{
    switch(pos)
    {
        case FIELD_POSITION::PLAYER_1_0:
            return "Player 1 Position 0";
        case FIELD_POSITION::PLAYER_2_0:
            return "Player 2 Position 0";
#if BATTLE_TYPE >= DOUBLE_BATTLE
        case FIELD_POSITION::PLAYER_1_1:
            return "Player 1 Position 1";
        case FIELD_POSITION::PLAYER_2_1:
            return "Player 2 Position 1";
#endif
#if BATTLE_TYPE == TRIPLE_BATTLE
        case FIELD_POSITION::PLAYER_1_2:
            return "Player 1 Position 2";
        case FIELD_POSITION::PLAYER_2_2:
            return "Player 2 Position 2";
#endif
        default:
            ERR_MSG("Unhandled Field Position\n");
    }
    return "";
}