//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_BATTLE_H
#define POKEMON_SIM_BATTLE_H

#include "Pokemon/Pokemon.h"
#include "Field.h"
#include "Battle/Party.h"

#include <vector>
#include <random>


enum class Attack_Result
{
    MISS = 0,
    FAINT,
    HIT,
    NO_PP,
    FLINCHED,
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

    void load_battle();
    void reset();
    void update_generator(long seed);


    bool send_out(FIELD_POSITION pos, int poke_position);
    void return_poke(FIELD_POSITION pos);
    bool swap_poke(FIELD_POSITION pos, int poke_position);
    bool can_swap(Players player);

    Party get_party(Players player);

    Attack_Result attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move);
    bool handle_end_turn_field_status();
    void reset_field_types();

    bool has_lost(Players player);
    void print_battle(bool detailed=false);
private:
    std::vector<std::string> select_teams();
    void load_teams(std::vector<std::string> team_names);
    void load_game_moves();

    Attack_Result attack_target(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move move, bool crit);
    std::pair<Attack_Result, float> attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move move, bool crit);
    int get_move_power(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, Move move);

    Attack_Result handle_pre_attack_status(FIELD_POSITION pos);
    Attack_Result handle_v_status_mask(FIELD_POSITION pos, int status_mask, int move_num = -1);
    Attack_Result handle_v_status(FIELD_POSITION pos, int v_status, int move_num);
    bool handle_end_turn_status(FIELD_POSITION pos);
    bool handle_end_turn_statuses(FIELD_POSITION pos);

    Attack_Result handle_move_effects(Effect move_effect, FIELD_POSITION atk_pos, FIELD_POSITION def_pos, float damage);

    void update_party(Players player);
    void handle_faint(FIELD_POSITION pos);

    bool roll_chance(float chance);
    bool roll_acc(float acc, float atk_acc_mod, float def_eva_mod);
    float calculate_damage_dealt(int attacker_level, int move_power, float atk, float def, float damage_modifier);
    float calculate_damage_modifier(Move move, Field field, Pokemon attacker, Pokemon defender, int num_targets, bool crit);

    std::mt19937 generator;

    Party Parties [2];
    Move game_moves [Game_Moves::NUM_GAME_MOVES];
};


#endif //POKEMON_SIM_BATTLE_H
