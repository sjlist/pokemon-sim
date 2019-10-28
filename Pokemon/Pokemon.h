//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_POKEMON_H
#define POKEMON_SIM_POKEMON_H

#include <Pokemon/Move.h>
#include <Pokemon/Natures.h>
#include <Pokemon/Stats.h>

#include <boost/property_tree/ptree.hpp>
#include <string>
using namespace std;

enum Active_State
{
    benched = 0,
    to_be_swapped,
    in_field
};

class Pokemon {
public:
    Pokemon();

    PokeTypes* get_type();
    int get_level();
    float get_stat(STAT stat);
    STATUS get_status();
    int get_volatile_status();
    float get_current_hp();
    int get_v_status_turns(VOLATILE_STATUS_NUMBERS v_status);
    int get_protect_turns();

    bool is_volatile_status(VOLATILE_STATUS v_status);
    bool is_active();
    bool is_benched();
    bool is_swapping();
    string get_species();
    bool is_alive();
    bool is_grounded();
    bool is_protected();
    bool can_mega();

    bool use_move(int move_number);
    void set_active();
    void set_benched();
    void set_swapping();
    bool deal_damage(float damage, bool ignore_sub = false);
    void heal_damage(int damage);
    bool set_status(STATUS new_status);
    bool set_volatile_status(VOLATILE_STATUS v_status);
    void protect_poke();
    void stat_change(STAT stat, int stages);
    void faint_poke();
    void increment_v_status_turns(VOLATILE_STATUS_NUMBERS v_status);
    void increment_protect_turns();
    void remove_type(int type_num);
    bool setup_substitute();
    void mega_evolve();

    void reset_types();
    void reset_protect();
    void clear_stat_mods();
    void clear_volatile_status(VOLATILE_STATUS v_status);
    void clear_volatile_status_mask(int v_status_mask);
    void clear_volatile_statuses();
    void clear_v_status_turns(VOLATILE_STATUS_NUMBERS v_status);
    void clear_protect_turns();

    int status_turns;
    bool first_turn;

    void print_pokemon(bool detailed=false);

    void reset();
    void load_pokemon(boost::property_tree::ptree poke_ptree);
    Move moves[4];

    //UNIT TEST HELPERS
    void create_test_pokemon(PokeTypes t1, PokeTypes t2, Natures n, float pcent_hp, string species="unit-test/Testemon");
private:
    bool alive;
    Active_State active;
    int level;
    PokeTypes type[2];
    PokeTypes current_type[2];
    float current_hp;
    string name;
    float* current_stats;
    float base_stats [STAT::NUM_STATS];
    int stat_modifiers [STAT::NUM_STATS] = {0, 0, 0, 0, 0, 0, 0, 0};
    int evs [STAT::NUM_STATS];
    int ivs [STAT::NUM_STATS];
    STATUS status;
    string species;
    bool protect_active;
    int protect_turns;
    Natures nature;

    bool has_mega;
    bool is_mega;
    PokeTypes mega_type[2];
    float mega_base_stats [STAT::NUM_STATS];

    float substitute_hp;

    int get_stat_mod(STAT stat);

    int volatile_status;
    int v_status_turns [NUM_VOLATILE_STATUS];

    void load_species(string species_name);
    void set_stats(bool use_mega = false);
    float calculate_hp(int level, int base_hp, int ev_hp, int iv_hp);
    float calculate_stat_single(int level, int base, int ev, int iv, float nature_mod);
};


#endif //POKEMON_SIM_POKEMON_H
