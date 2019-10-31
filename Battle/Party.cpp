//
// Created by slist on 1/8/19.
//

#include <Battle/Party.h>
#include <Pokemon/Pokemon.h>
#include <Utils/Logging.h>

#include <cmath>

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

void Party::print_party()
{
    for(int i = 0; i < 6; i++)
    {
        DEBUG_MSG(Party::party_pokes[i].get_species() << ": " <<
                                                      round(Party::party_pokes[i].get_current_hp() /
                                                            Party::party_pokes[i].get_stat(STAT::HP) *
                                                            100 * 10) / 10 << "%");
        if (Party::party_pokes[i].is_active())
            DEBUG_MSG(" ACTIVE");
        DEBUG_MSG(endl);
    }
}