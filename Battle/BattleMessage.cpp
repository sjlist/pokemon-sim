//
// Created by slist on 6/24/19.
//

#include "BattleMessage.h"

BattleMessage::BattleMessage()
{
    move_command = Commands::COMMAND_NONE;
    pos = FIELD_POSITION::NO_POSITION;
    target_pos = FIELD_POSITION::NO_POSITION;
    active_pos  = FIELD_POSITION::NO_POSITION;
    move_num = 0;
    reserve_poke = 0;
    team_name = "";
    mega_evolve = false;
}

void BattleMessage::set_no_op()
{
    BattleMessage::move_command = Commands::COMMAND_NONE;
    BattleMessage::mega_evolve = false;
}
