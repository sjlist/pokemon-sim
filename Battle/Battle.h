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
    NUM_ATTACK_RESLUTS
};

class Battle {
public:
    Battle();
    Field active_field;
    void load_battle();
    void send_out(Players player, int poke_position);

    Attack_Result attack(Players player, int move);
    void print_battle(bool detailed=false);
private:
    std::vector<std::string> select_teams();

    void load_teams(std::vector<std::string> team_names);

    bool roll_chance(float chance);
    bool roll_acc(float acc);
    int calculate_damaage_dealt(int attacker_level, int move_power, int atk, int def, float damage_modifier);
    float calculate_damage_modifier(Move move, Field field, Pokemon attacker, Pokemon defender, int num_targets);

    Pokemon p1_party [6];
    Pokemon p2_party [6];
};


#endif //POKEMON_SIM_BATTLE_H
