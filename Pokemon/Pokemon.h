//
// Created by slist on 12/12/18.
//

#ifndef POKEMON_SIM_POKEMON_H
#define POKEMON_SIM_POKEMON_H

#include "Move.h"
#include "Natures.h"
#include <string>
#include <boost/property_tree/ptree.hpp>

class Pokemon {
public:
    Pokemon();

    PokeTypes* get_type();
    int get_level();
    int get_stat(STAT stat);
    STATUS get_status();
    bool is_active();
    std::string get_species();
    bool is_alive();

    bool use_move(int move_number);
    void set_active(bool state);
    bool deal_damage(int damage);
    void set_status(STATUS new_status);
    void stat_change(STAT stat, int stages);
    void faint_poke();

    int status_turns;

    void print_pokemon(bool detailed=false);

    void load_pokemon(boost::property_tree::ptree poke_ptree);
    void set_stats(int* base_stats, int* ivs, int* evs, int level, Natures nature);
    Move moves[4];
private:
    bool alive;
    bool active;
    int level;
    PokeTypes type[2];
    int current_hp;
    std::string name;
    int base_stats [6];
    int stat_modifiers [6] = {0, 0, 0, 0, 0, 0};
    STATUS status;
    std::string species;

    int* load_species(std::string species_name);
    int calculate_hp(int level, int base_hp, int ev_hp, int iv_hp);
    int calculate_stat_single(int level, int base, int ev, int iv, float nature_mod);
};


#endif //POKEMON_SIM_POKEMON_H
