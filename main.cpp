//
// Created by slist on 12/12/18.
//

#include <Battle/BattleStateMachine.h>
#include <Battle/BattleActor.h>
#include <Utils/Logging.h>
#include <iostream>
#include <chrono>

#include "Player_Config.h"

using namespace std;

struct GuessData
{
    float guess, guess_confidence, time_elasped, run_variance, time_variance;
};

string seconds_to_time_string(float total_time);
BattleMessage request_message_from_actor(BattleNotification note, FIELD_POSITION pos, BattleActor* actors, Battle* battle);
GuessData guess_time_left(chrono::duration<double> time_elasped, float time_since_last_guess, float last_guess, int max_runs, int num_runs);
void check_probability(float win_pcent, int num_runs);

#define DELTA 0.001

int main()
{
    int max_runs = 500000, round_dec = 2, rounding_val;
    int num_runs = 0, winner, max_turns = 0, tot_turns = 0;
    GuessData guess_data;
    float current_pcent = 0, time_since_last_guess, seconds_per_battle, wins [3] = {0, 0, 0};
    chrono::duration<double> time_elasped;
    rounding_val = pow(10, round_dec);
    vector<GuessData> guesses;

    BattleStateMachine BSM;
    pair<BattleNotification, FIELD_POSITION> res = make_pair(BattleNotification::TEAM_CHOICE, PLAYER_1_0);
    BattleActor actors [2];
    long seed;
    BattleMessage message;

    BSM.update_random_seed(RAND_SEED);

    auto start_time = chrono::system_clock::now();
    auto current_time = chrono::system_clock::now();

    while(num_runs < max_runs)
    {
        num_runs++;
        if(round((float)num_runs/max_runs*rounding_val)/rounding_val > current_pcent)
        {
            current_pcent = round((float)num_runs/max_runs*rounding_val)/rounding_val;
            current_time = chrono::system_clock::now();

            time_elasped = (current_time - start_time);
            time_since_last_guess = (time_elasped.count() - guess_data.time_elasped);

            guess_data = guess_time_left(time_elasped, time_since_last_guess, guess_data.guess, max_runs, num_runs);

            guesses.push_back(guess_data);

            cout << current_pcent*100 << "% done"
                 << " Expected time remaining: " << seconds_to_time_string(guess_data.guess)
//                 << " Guess confidence: " << guess_data.guess_confidence
                 << endl;
        }

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
    current_time = chrono::system_clock::now();
    time_elasped = (current_time - start_time);
    seconds_per_battle = time_elasped.count() / max_runs;

    for(unsigned int i = 0; i < 3; i++)
    {
        wins[i] = wins[i] / max_runs * 100;
    }

    cout << endl;
    cout << "Player 1 won " << wins[2] << "% of the time\n";
    cout << "Player 2 won " << wins[0] << "% of the time\n";
    check_probability(wins[0], num_runs);
    cout << "Max turn count " << max_turns << "\n";
    cout << "Average turn count " << tot_turns / (float)max_runs << "\n";
    cout << "Average time to execute one battle: " << seconds_per_battle << " s\n";
    cout << "Execution time: " << seconds_to_time_string(time_elasped.count()) << endl;

//    while(!guesses.empty())
//    {
//        guess_data = guesses.back();
//
//        if(guess_data.time_elasped == 0)
//            break;
//
//        guess_error = abs(time_elasped.count() - (guess_data.time_elasped + guess_data.guess)) / time_elasped.count();
//        cout << "Guess at " << seconds_to_time_string(guess_data.time_elasped)
//             << " was " << seconds_to_time_string(guess_data.guess)
//             << " was off by " << guess_error * 100 << "%"
//             << " With a confidence: " << guess_data.guess_confidence
//             << " Run Variance: " << guess_data.run_variance
//             << " Time Variance: " << guess_data.time_variance
//             << endl;
//        guesses.pop_back();
//    }

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
            return actors[action_player].choose_action(pos, battle->get_party(action_player), &battle->active_field, battle->can_mega(pos), Actions::CHOOSE_ACTION);
        case BattleNotification::FORCE_POKEMON_SWAP:
            m.move_command = COMMAND_SWAP;
            m.reserve_poke = actors[action_player].choose_pokemon(battle->get_party(action_player), true);
            m.pos = pos;
            return m;
        case BattleNotification::POKEMON_SWAP:
            m.move_command = COMMAND_SWAP;
            m.reserve_poke = actors[action_player].choose_pokemon(battle->get_party(action_player));
            m.pos = pos;
            return m;
        default:
            ERR_MSG("Unhandled Notification");
    }

    if(m.mega_evolve)
        battle->mega_pending(m.pos);

    return m;
}

string seconds_to_time_string(float total_time)
{
    float second_fractions, seconds;
    int minutes, hours, days, remaining_time;
    string time_string = "";


    second_fractions = total_time - floor(total_time);
    remaining_time = floor(total_time); //remaining_time is now in whole seconds

    seconds = remaining_time % 60 + second_fractions;
    remaining_time = remaining_time / 60; //remaining_time is now in whole minutes
    minutes = remaining_time % 60;
    remaining_time = remaining_time / 60; //remaining_time is now in whole hours
    hours = remaining_time % 24;
    days = remaining_time / 24;

    if(days > 0)
    {
        time_string = time_string + to_string(days) + " days ";
    }

    if(hours > 0)
    {
        time_string = time_string + to_string(hours) + " hours ";
    }

    if(minutes > 0)
    {
        time_string = time_string + to_string(minutes) + " minutes ";
    }

    if(seconds > 0)
    {
        time_string = time_string + to_string(seconds) + " seconds";
    }

    return time_string;
}

GuessData guess_time_left(chrono::duration<double> time_elasped, float time_since_last_guess, float last_guess, int max_runs, int num_runs)
{
    float current_guess = 0, guess_diff = 0, guess_confidence = 0, guess_time_variance = 0, guess_run_variance = 0;
    GuessData data;

    current_guess = time_elasped.count() * (max_runs - num_runs) / num_runs;

    guess_diff = abs(current_guess - last_guess);

    //guess_time_variance = abs(2 - (time_since_last_guess / abs(guess_diff)));
    guess_time_variance = abs(1 - (time_since_last_guess + guess_diff) / (time_elasped.count() + current_guess));
    guess_run_variance = 1;//abs(1 - ((float)max_runs / (rounding_val * num_runs)));
    guess_confidence = guess_time_variance * guess_run_variance;

    data.guess_confidence = guess_confidence;
    data.guess = current_guess;
    data.time_elasped = time_elasped.count();
    data.run_variance = guess_run_variance;
    data.time_variance = guess_time_variance;

    return data;
}

void check_probability(float win_pcent, int num_runs)
{
    float expected = 50, diff, thresh;
    diff = abs(expected - win_pcent)/100;

    thresh = sqrt(log(2 * 2/DELTA)/(2 * num_runs));
    cout << "Threshold is: " << thresh*100 << "%" << endl;
    if(thresh < diff)
        cout << "There is an error with your win percentage " << thresh*100 << "% < " << diff*100 << "%" << endl;
}