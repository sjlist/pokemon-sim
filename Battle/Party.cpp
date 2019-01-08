//
// Created by slist on 1/8/19.
//

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