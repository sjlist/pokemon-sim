//
// Created by slist on 18/1/19.
//

#include <Battle/BattleStateMachine.h>
#include <Battle/BattleActor.h>
#include <Utils/Logging.h>
#include <iostream>
#include <chrono>

#include "Player_Config.h"

using namespace std;

BattleMessage request_message_from_actor(BattleNotification note, FIELD_POSITION pos, BattleActor* actors, Battle* battle);


int main()
{
    int max_runs = 1000000;
    int num_runs = 0;

    BattleStateMachine BSM;
    pair<BattleNotification, FIELD_POSITION> res = make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
    BattleActor actors [2];
    long seed;
    BattleMessage message;

    BSM.update_random_seed(RAND_SEED);

    while(num_runs < max_runs)
    {
        num_runs++;

        seed = BSM.get_seed();
        actors[PLAYER_ONE].update_generator(seed);
        actors[PLAYER_TWO].update_generator(seed);

        while(res.first != BattleNotification::PLAYER_LOST)
        {
            res = BSM.run(message);
            message = request_message_from_actor(res.first, res.second, actors, BSM.get_battle());
        }

        res.first = BattleNotification::POKEMON_ACTION; // set res.first to be something other than PLAYER_LOST
        message.set_no_op();
        message.pos = FIELD_POSITION::NO_POSITION;
        BSM.reset();
    }
}

BattleMessage request_message_from_actor(BattleNotification note, FIELD_POSITION pos, BattleActor* actors, Battle* battle)
{
    BattleMessage m;

    if(note == BattleNotification::PLAYER_LOST)
        return m;

    Players action_player = get_player_from_position(pos);
    switch(note)
    {
        case BattleNotification::TEAM_CHOICE:
            if(get_player_from_position(pos) == PLAYER_ONE)
                m.team_name = TEAM_1_CHOICE;
            else
                m.team_name = TEAM_2_CHOICE;
            m.pos = pos;
            m.move_command = Commands::COMMAND_TEAM_CHOICE;
            return m;
        case BattleNotification::POKEMON_ACTION:
            return actors[action_player].choose_action(pos, battle->get_party(action_player), &battle->active_field, Actions::CHOOSE_ACTION);
        case BattleNotification::POKEMON_SWAP:
            m.move_command = COMMAND_SWAP;
            m.reserve_poke = actors[action_player].choose_pokemon(battle->get_party(action_player));
            m.pos = pos;
            return m;
        default:
            ERR_MSG("Unhandled Notification");
    }
    return m;
}