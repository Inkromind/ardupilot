/*
 * AMW_Commands_Plan.cpp
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Commands_Plan.h"
#include <AC_Facade.h>

void AMW_Commands_Plan::addNewCommand(AMW_Command* command) {
    if (!command)
        return;
    subCommands.push(command);
}

bool AMW_Commands_Plan::hasStarted(void) const {
    if (completed || subCommands.empty())
        return commandStarted;
    else
        return subCommands.front()->hasStarted();
}
