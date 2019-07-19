//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_MOVE_H
#define POKEMON_SIM_MOVE_H

#include <Battle/Targeting.h>
#include <Pokemon/Effect.h>
#include <Pokemon/Status.h>
#include <Pokemon/Type.h>

#include <string>
using namespace std;

static const int MAX_PRIO = 5;
static const int FAINT_PRIO = -10;
static const int MAX_EFFECTS = 4;

enum move_damage_type
{
    NO_DAMAGE_TYPE = 0, //BAD, put here for poor move intiailization
    MOVE_PHYSICAL,
    MOVE_SPECIAL,
    MOVE_STATUS,
    NUM_MOVE_DAMAGE_TYPES
};


class Move {
public:
    Move();

    bool use();
    int get_pp();
    int get_power();
    float get_acc();
    float get_crit();
    Effect get_move_effect(int effect_num);
    PokeTypes get_type();
    string get_name();
    int get_priority();
    TARGETS get_move_targets();
    int get_num_targets();
    int get_num_hits();
    bool makes_contact();
    bool ignores_protect();
    bool must_be_used_first_turn();

    void reset();

    void load_move(string move_name);
    static void print_move();
    move_damage_type get_damage_type();

private:
    string name;
    TARGETS move_targets;
    int num_targets;
    int max_pp;
    int power;
    float acc;
    int priority;
    array<Effect, MAX_EFFECTS> move_effects;
    float crit_chance;
    PokeTypes type;
    move_damage_type damage_type;
    int last_effect;
    bool contact;
    bool protectable;
    int num_hits;

    int current_pp;
    bool first_turn;
};


#endif //POKEMON_SIM_MOVE_H
