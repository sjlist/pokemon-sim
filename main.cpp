//
// Created by slist on 12/12/18.
//

#include <iostream>
#include "Pokemon/Type.h"
#include "Pokemon/Pokemon.h"
#include "Battle/Battle.h"
#include "Battle/BattleStateMachine.h"
#include "Pokemon/Move.h"

int main()
{
    BattleStateMachine BSM;
    BSM.run();

    return 1;
}
