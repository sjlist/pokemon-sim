//
// Created by slist on 12/13/18.
//

#include "Natures.h"
#include "Stats.h"

float get_nature_mod(Natures nature, STAT stat)
{
    switch(nature)
    {
        //ATTACK increase natures
        case Natures::LONLEY:
            if(stat == STAT::ATK)
            {
                return 1.1;
            }
            if(stat == STAT::DEF)
            {
                return 0.9;
            }
            return 1;
        case Natures::BRAVE:
            if(stat == STAT::ATK)
            {
                return 1.1;
            }
            if(stat == STAT::SPE)
            {
                return 0.9;
            }
            return 1;
        case Natures::ADAMANT:
            if(stat == STAT::ATK)
            {
                return 1.1;
            }
            if(stat == STAT::SPA)
            {
                return 0.9;
            }
            return 1;
        case Natures::NAUGHTY:
            if(stat == STAT::ATK)
            {
                return 1.1;
            }
            if(stat == STAT::SPD)
            {
                return 0.9;
            }
            return 1;


        //DEFENSE increasing natures
        case Natures::BOLD:
            if(stat == STAT::DEF)
            {
                return 1.1;
            }
            if(stat == STAT::ATK)
            {
                return 0.9;
            }
            return 1;
        case Natures::RELAXED:
            if(stat == STAT::DEF)
            {
                return 1.1;
            }
            if(stat == STAT::SPE)
            {
                return 0.9;
            }
            return 1;
        case Natures::IMPISH:
            if(stat == STAT::DEF)
            {
                return 1.1;
            }
            if(stat == STAT::SPA)
            {
                return 0.9;
            }
            return 1;
        case Natures::LAX:
            if(stat == STAT::DEF)
            {
                return 1.1;
            }
            if(stat == STAT::SPD)
            {
                return 0.9;
            }
            return 1;


        //SPEED increasing natures
        case Natures::TIMID:
            if(stat == STAT::SPE)
            {
                return 1.1;
            }
            if(stat == STAT::ATK)
            {
                return 0.9;
            }
            return 1;
        case Natures::HASTY:
            if(stat == STAT::SPE)
            {
                return 1.1;
            }
            if(stat == STAT::DEF)
            {
                return 0.9;
            }
            return 1;
        case Natures::JOLLY:
            if(stat == STAT::SPE)
            {
                return 1.1;
            }
            if(stat == STAT::SPA)
            {
                return 0.9;
            }
            return 1;
        case Natures::NAIVE:
            if(stat == STAT::SPE)
            {
                return 1.1;
            }
            if(stat == STAT::SPD)
            {
                return 0.9;
            }
            return 1;


        //SPECIAL ATTACK increasing natures
        case Natures::MODEST:
            if(stat == STAT::SPA)
            {
                return 1.1;
            }
            if(stat == STAT::ATK)
            {
                return 0.9;
            }
            return 1;
        case Natures::MILD:
            if(stat == STAT::SPA)
            {
                return 1.1;
            }
            if(stat == STAT::DEF)
            {
                return 0.9;
            }
            return 1;
        case Natures::QUIET:
            if(stat == STAT::SPA)
            {
                return 1.1;
            }
            if(stat == STAT::SPE)
            {
                return 0.9;
            }
            return 1;
        case Natures::RASH:
            if(stat == STAT::SPA)
            {
                return 1.1;
            }
            if(stat == STAT::SPD)
            {
                return 0.9;
            }
            return 1;


        //SPECIAL DEFENSE increasing natures
        case Natures::CALM:
            if(stat == STAT::SPD)
            {
                return 1.1;
            }
            if(stat == STAT::ATK)
            {
                return 0.9;
            }
            return 1;
        case Natures::GENTLE:
            if(stat == STAT::SPD)
            {
                return 1.1;
            }
            if(stat == STAT::DEF)
            {
                return 0.9;
            }
            return 1;
        case Natures::SASSY:
            if(stat == STAT::SPD)
            {
                return 1.1;
            }
            if(stat == STAT::SPE)
            {
                return 0.9;
            }
            return 1;
        case Natures::CAREFUL:
            if(stat == STAT::SPD)
            {
                return 1.1;
            }
            if(stat == STAT::SPA)
            {
                return 0.9;
            }
            return 1;

        //Neutral natures
        default:
            return 1.0;
    }
}