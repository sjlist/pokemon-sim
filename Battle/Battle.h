//
// Created by slist on 12/13/18.
//

#ifndef POKEMON_SIM_BATTLE_H
#define POKEMON_SIM_BATTLE_H

#include "Pokemon/Pokemon.h"
#include "Field.h"
#include <vector>

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

class Party {
public:
    Party();
    Pokemon party_pokes [6];
};

class Battle {
public:
    Battle();
    Field active_field;
    void load_battle();
    void send_out(FIELD_POSITION pos, int poke_position);
    void return_poke(FIELD_POSITION pos);
    void swap_poke(FIELD_POSITION pos, int poke_position);
    bool can_swap(Players player);


    Party get_party(Players player);

    Attack_Result attack(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move);
    bool handle_end_turn_field_status();

    bool has_lost(Players player);
    void print_battle(bool detailed=false);
private:
    std::vector<std::string> select_teams();
    void load_teams(std::vector<std::string> team_names);

    Attack_Result attack_damage(FIELD_POSITION atk_pos, FIELD_POSITION def_pos, int move_number);

    bool handle_pre_attack_status(FIELD_POSITION pos);
    bool handle_pre_attack_v_statuses(FIELD_POSITION pos);
    bool handle_pre_attack_v_status(FIELD_POSITION pos, int v_status);
    bool handle_end_turn_status(FIELD_POSITION pos);

    Attack_Result handle_move_effects(Effect move_effect, FIELD_POSITION atk_pos, FIELD_POSITION def_pos);

    void update_party(Players player);
    void handle_faint(FIELD_POSITION pos);

    bool roll_chance(float chance);
    bool roll_acc(float acc, float atk_acc_mod, float def_eva_mod);
    int calculate_damage_dealt(int attacker_level, int move_power, int atk, int def, float damage_modifier);
    float calculate_damage_modifier(Move move, Field field, Pokemon attacker, Pokemon defender, int num_targets);

    Party Parties [2];
};


#endif //POKEMON_SIM_BATTLE_H
