//
// Created by slist on 6/24/19.
//

#include "BattleMessage.h"

BattleMessage::BattleMessage() = default;

void BattleMessage::set_no_op()
{
    BattleMessage::move_command = Commands::COMMAND_NONE;
}
