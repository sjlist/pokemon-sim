//
// Created by slist on 1/8/19.
//

#include <iostream>
#include "Party.h"
#include "Pokemon/Pokemon.h"

Party::Party() = default;

void Party::reset()
{
    for(int i = 0; i < 6; i++)
    {
        if(Party::party_pokes[i].get_species().empty())
            break;
        else
            Party::party_pokes[i].reset();
    }
}

void Party::print_party(bool detailed)
{
    for(int i = 0; i < 6; i++)
        if (detailed)
            Party::party_pokes[i].print_pokemon(detailed);
        else if(!Party::party_pokes[i].is_active())
        {
            std::cout << Party::party_pokes[i].get_species() << ": " <<
                      round(Party::party_pokes[i].get_current_hp() / Party::party_pokes[i].get_stat(STAT::HP) *
                            100 * 10) / 10 << "%\n";
        }
        else
            std::cout << Party::party_pokes[i].get_species() << ": ACTIVE\n";
}