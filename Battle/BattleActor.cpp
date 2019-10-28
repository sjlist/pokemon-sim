//
// Created by slist on 12/19/18.
//

#include <Battle/Battle.h>
#include <Battle/BattleActor.h>
#include <Utils/Logging.h>

#include <stdlib.h>
#include <string>
#include <time.h>
using namespace std;

//BATTLE ACTOR IS CURRENTLY IMPLEMENTED AS AN ALL RANDOM PROCESS. CHOOSING ANYTHING FROM A LIST OF OPTIONS

BattleActor::BattleActor()
{
    BattleActor::pcent_chance = uniform_real_distribution<float>{0, 1};
    BattleActor::poke_choice = uniform_int_distribution<int> {0, 599};
}

BattleActor::BattleActor(long seed)
{
    BattleActor::generator = mt19937(seed);
    BattleActor::pcent_chance = uniform_real_distribution<float>{0, 1};
    BattleActor::poke_choice = uniform_int_distribution<int> {0, 599};
}

BattleMessage BattleActor::choose_action(FIELD_POSITION pos, Party* player_party, Field* field, bool player_can_mega, Actions action)
{
    BattleMessage message;
    Players player = get_player_from_position(pos);
    Move move;

    message.pos = pos;

    if(field->active_pokes[pos] == nullptr)
    {
        message.set_no_op();
        return message;
    }

    if(action == Actions::CHOOSE_ACTION)
    {
        if(BattleActor::roll_chance(BattleActor::attack_swap_ratio[player]))
        {
            action = Actions::CHOOSE_MOVE;
        }
        else
        {
            action = Actions::CHOOSE_POKEMON;
        }
    }

    switch(action)
    {
        case Actions::CHOOSE_POKEMON:
            message.move_command = Commands::COMMAND_SWAP;
            message.reserve_poke = BattleActor::choose_pokemon(player_party);
            message.active_pos = pos;
            if(message.reserve_poke == -1)
                action = Actions::CHOOSE_MOVE;
            else
                break;

        case Actions::CHOOSE_MOVE:
            message.move_command = Commands::COMMAND_ATTACK;
            message.move_num = BattleActor::choose_move(field->active_pokes[pos]);
            if(message.move_num != 5)
            {
                move = field->active_pokes[pos]->moves[message.move_num];
                message.target_pos = BattleActor::choose_target(pos, move.get_num_targets(), move.get_move_targets(), field);

            }
            else
            {
                message.target_pos = BattleActor::choose_target(pos, 1, TARGETS::ADJACENT_ENEMY, field);
            }
            break;
        default:
            ERR_MSG("Unhandled Action");
    }

    DEBUG_MSG("Player " << get_player_from_position(pos) + 1 << " is ");

    // Decide if the pokemon is going to mega evolve
    if(field->active_pokes[pos]->can_mega() && player_can_mega)
    {
        message.mega_evolve = true;
        DEBUG_MSG("mega evolving " <<  field->active_pokes[pos]->get_species() << "and ");
    }

    if(message.move_command == Commands::COMMAND_SWAP)
    {
        DEBUG_MSG("swapping position: " << get_string_from_field_position(pos) << ", sending out " << player_party->party_pokes[message.reserve_poke].get_species() << "\n");
    }
    else if(message.move_command == Commands::COMMAND_ATTACK)
    {
        DEBUG_MSG("attacking with " << field->active_pokes[pos]->get_species() << ", targeting ");
        if(message.target_pos != FIELD_POSITION::ALL_TARGETS)
        {
            DEBUG_MSG("P" << get_player_from_position(message.target_pos) + 1 << "'s ");
            DEBUG_MSG(field->active_pokes[message.target_pos]->get_species());
        }
        else
        {
            DEBUG_MSG("ALL TARGETS");
        }

        DEBUG_MSG(" with ");

        if(message.move_num < 4)
            DEBUG_MSG(field->active_pokes[pos]->moves[message.move_num].get_name() << "\n");
        else
            DEBUG_MSG("struggle\n");
    }
    else
        ERR_MSG("Unhandled Command Type" << message.move_command << endl);

    return message;
}

int BattleActor::choose_move(Pokemon* poke)
{
    int num_moves = 0, selection;
    int moves [4];
    for(int i = 0; i < 4; i++)
    {
        if(poke->moves[i].get_pp() != 0)
        {
            moves[num_moves] = i;
            num_moves++;
        }
    }

    if(num_moves == 0)
        return 5;

    selection = BattleActor::choose_position(num_moves);
    return moves[selection];
}

int BattleActor::choose_pokemon(Party* party, bool force_swap)
{
    int num_pokes = 0, selection;
    int pokes [6];
    for(int i = 0; i < 6; i++)
    {
        if(party->party_pokes[i].is_alive()
        && party->party_pokes[i].is_benched())
        {
            pokes[num_pokes] = i;
            num_pokes++;
        }
    }

    // If there are no other pokemon we can swap with, allow the actor to chose a pokemon that is slated to be swapped
    if(num_pokes == 0)
    {
        if(force_swap)
        {
            for (int i = 0; i < 6; i++)
            {
                if (party->party_pokes[i].is_alive()
                    && !party->party_pokes[i].is_active())
                {
                    pokes[num_pokes] = i;
                    num_pokes++;
                }
            }
            if(num_pokes == 0)
                return -1;
        }
        else
            return -1;
    }

    selection = BattleActor::choose_position(num_pokes);
    party->party_pokes[pokes[selection]].set_swapping();
    return pokes[selection];
}

FIELD_POSITION BattleActor::choose_target(FIELD_POSITION atk_pos, int num_targets, TARGETS targets, Field* field)
{

    if(num_targets == 1)
    {
        BattleActor::actor_targeting.get_valid_targets(targets, atk_pos);
        FIELD_POSITION alive_targets [FIELD_POSITION::NUM_POSITIONS];
        int num_alive_targets = 0;

        //determine which of the valid move targets are actually alive
        for(int i = 0; i < BattleActor::actor_targeting.get_num_valid_targets(); i++)
        {
            if(field->active_pokes[BattleActor::actor_targeting.valid_targets[i]] != nullptr)
            {
                alive_targets[num_alive_targets] = BattleActor::actor_targeting.valid_targets[i];
                num_alive_targets++;
            }
        }

        // choose a pokemon who is alive
        int rand_target = BattleActor::choose_position(num_alive_targets);
        return alive_targets[rand_target];
    }
    else
    {
        return FIELD_POSITION::ALL_TARGETS;
    }
}

bool BattleActor::roll_chance(float chance)
{
    float c = BattleActor::pcent_chance(BattleActor::generator);
    return c < chance;
}

int BattleActor::choose_position(int num_positions)
{
    if(num_positions > 6)
        ERR_MSG("Choose position can only support up to 6 choices");

    float c = BattleActor::poke_choice(BattleActor::generator);
    return floor(c/(600/num_positions));
}

void BattleActor::update_generator(long seed)
{
    BattleActor::generator = mt19937(seed);
}