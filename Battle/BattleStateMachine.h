//
// Created by slist on 12/19/18.
//

#ifndef POKEMON_SIM_BATTLESTATEMACHINE_H
#define POKEMON_SIM_BATTLESTATEMACHINE_H

#include <Battle/Battle.h>
#include <Battle/BattleMessage.h>
#include <vector>
using namespace std;

#define MAX_SPEED 10000

enum class BattleState
{
    BATTLE_INIT = 0,
    BATTLE_START,
    TURN_START,
    ACTION_REQUEST,
    TURN_EXECUTE,
    TURN_END_STATUS_CHECK,
    TURN_END_FAINT_CHECK,
    BATTLE_END,
    BATTLE_IDLE,
    NUM_BATTLE_STATES
};

enum class BattleNotification
{
    POKEMON_SWAP,
    POKEMON_ACTION,
    PLAYER_LOST,
    TEAM_CHOICE
};

class BattleStateMachine {
public:
    BattleStateMachine();

    void update_random_seed(long seed);
    void update_random_seed();

    pair<BattleNotification, FIELD_POSITION> run(BattleMessage message);
    Battle* get_battle();
    int get_turn_count();
    long get_seed();
    void reset();
    int get_battle_result();

protected:
    void sort_message_stack();
    vector<FIELD_POSITION> create_speed_list();
    BattleState state;

    vector<BattleMessage> turn_messages;
private:
    void init(Players player, string team_name);

    long seed;
    mt19937 choice;
    int num_players = 2;
    int turn_count;
    Battle battle;

    bool battle_over();

    void validate_battle_message(BattleMessage message);

    vector<FIELD_POSITION> speed_list;

    int make_choice(int min, int max);

    void remove_message_from_stack(FIELD_POSITION pos);
};


#endif //POKEMON_SIM_BATTLESTATEMACHINE_H
