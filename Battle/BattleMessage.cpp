//
// Created by slist on 6/24/19.
//

#include "BattleMessage.h"

void BattleMessage::set_no_op()
{
    BattleMessage::move_command = Commands::COMMAND_NONE;
}
