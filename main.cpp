//
// Created by slist on 12/12/18.
//

#include <Battle/BattleStateMachine.h>
#include <Battle/BattleActor.h>
#include <Utils/Logging.h>
#include <iostream>
using namespace std;

BattleMessage request_message_from_actor(BattleNotification note, FIELD_POSITION pos, BattleActor* actors, Battle* battle)
{
    BattleMessage m;

    if(note == BattleNotification::PLAYER_LOST)
        return m;

    Players action_player = get_player_from_position(pos);
    switch(note)
    {
        case BattleNotification::TEAM_CHOICE:
            m.team_name = "team1";
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
}

int main()
{
    int wins [3] = {0, 0, 0}, max_runs = 10000;
    int num_runs = 0, winner, max_turns = 0, tot_turns = 0;

    BattleStateMachine BSM;
    pair<BattleNotification, FIELD_POSITION> res = make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
    BattleActor actors [2];
    long seed;
    BattleMessage message;

    BSM.update_random_seed(RAND_SEED);

    while(num_runs < max_runs)
    {
        num_runs++;
        cout <<"RUN: " << num_runs << endl;
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
        winner = BSM.get_battle_result() + 1;
        wins[winner] += 1;
        if(BSM.get_turn_count() > max_turns)
            max_turns = BSM.get_turn_count();
        tot_turns += BSM.get_turn_count();
        BSM.reset();
    }

    cout << endl;
    cout << "Player 1 won " << wins[2] / (float)max_runs * 100 << "% of the time\n";
    cout << "Player 2 won " << wins[0] / (float)max_runs * 100 << "% of the time\n";
    cout << "Max turn count " << max_turns << "\n";
    cout << "Average turn count " << tot_turns / (float)max_runs << "\n";
}
