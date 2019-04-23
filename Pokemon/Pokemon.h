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
    std::string get_species();
    bool is_alive();
    bool is_grounded();
    bool is_protected();

    bool use_move(int move_number);
    void set_active(bool state);
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


    void reset_types();
    void reset_protect();
    void clear_stat_mods();
    void clear_volatile_status(VOLATILE_STATUS v_status);
    void clear_volatile_status_mask(int v_status_mask);
    void clear_volatile_statuses();
    void clear_v_status_turns(VOLATILE_STATUS_NUMBERS v_status);
    void clear_protect_turns();

    int status_turns;
    bool to_be_swapped;
    bool first_turn;

    void print_pokemon(bool detailed=false);

    void reset();
    void load_pokemon(boost::property_tree::ptree poke_ptree);
    Move moves[4];
private:
    bool alive;
    bool active;
    int level;
    PokeTypes type[2];
    PokeTypes current_type[2];
    float current_hp;
    std::string name;
    float base_stats [STAT::NUM_STATS];
    int stat_modifiers [STAT::NUM_STATS] = {0, 0, 0, 0, 0, 0, 0, 0};
    STATUS status;
    std::string species;
    bool protect_active;
    int protect_turns;

    float substitute_hp;

    int get_stat_mod(STAT stat);

    int volatile_status;
    int v_status_turns [NUM_VOLATILE_STATUS];

    void load_species(std::string species_name);
    void set_stats(int* ivs, int* evs, int level, Natures nature);
    float calculate_hp(int level, int base_hp, int ev_hp, int iv_hp);
    float calculate_stat_single(int level, int base, int ev, int iv, float nature_mod);
};


#endif //POKEMON_SIM_POKEMON_H
