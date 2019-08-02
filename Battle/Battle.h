//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_BATTLE_H
#define POKEMON_SIM_BATTLE_H

#include <Battle/Field.h>
#include <Battle/Party.h>
#include <Pokemon/Pokemon.h>
#include <Battle/Players.h>

#include <vector>
#include <random>
using namespace std;

enum class Attack_Result
{
    MISS = 0,
    FAINT,
    HIT,
    NO_PP,
    SWAP,
    IMMUNE,
    NUM_ATTACK_RESLUTS,
    NO_ATTACK
};

enum Game_Moves
{
    MOVE_STRUGGLE,
    MOVE_CONFUSION,
    NUM_GAME_MOVES
};

class Battle {
public:
    Battle();
    explicit Battle(long seed);

    Field active_field;
    Targets Battle_Targets;

    void load_battle(Players player, string* team_name);
    void reset();
    void update_generator(long seed);


    Attack_Result send_out(FIELD_POSITION pos, int poke_position);
    void return_poke(FIELD_POSITION pos);
    Attack_Result swap_poke(FIELD_POSITION pos, int poke_position);
    bool can_swap(Players player);

    Party* get_party(Players player);

    Attack_Result attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move);
    bool handle_end_turn_statuses(FIELD_POSITION pos);
    bool handle_end_turn_field_status();
    void reset_temp_field_status();

    bool has_lost(Players player);
    void print_battle();

protected:
    bool roll_chance(float chance);

    int get_move_power(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move);
    int calculate_damage_dealt(int attacker_level, int move_power, int atk, int def, vector<float>* damage_mods);
    void calculate_damage_modifier(vector<float>* mults,Move* move, Pokemon* attacker, Pokemon* defender, int num_targets, bool crit);
    int do_chain_mult(int base_power, vector<float>* mults);

    uniform_int_distribution<int>damage_calc;
    mt19937 generator;

    Party Parties [2];

private:
    void load_teams(Players player, string team_name);
    int count_pokemon_team(boost::property_tree::ptree team);
    void load_game_moves();

    Attack_Result attack_target(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move);
    pair<Attack_Result, float> attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move* move);

    Attack_Result handle_contact(FIELD_POSITION attacker, FIELD_POSITION defender);
    Attack_Result handle_pre_attack_status(FIELD_POSITION pos);
    Attack_Result handle_v_status_mask(FIELD_POSITION pos, int status_mask, int move_num = -1);
    Attack_Result handle_v_status(FIELD_POSITION pos, int v_status, int move_num);
    bool handle_end_turn_status(FIELD_POSITION pos);

    Attack_Result handle_move_effects(Effect move_effect, FIELD_POSITION atk_pos, FIELD_POSITION def_pos, float damage);

    void handle_faint(FIELD_POSITION pos);

    bool roll_acc(float acc, float atk_acc_mod, float def_eva_mod);

    uniform_real_distribution<float> chance;

    Move game_moves [Game_Moves::NUM_GAME_MOVES];
};


#endif //POKEMON_SIM_BATTLE_H
