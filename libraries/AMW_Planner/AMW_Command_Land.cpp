/*
 * AMW_Command_Land.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Land.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Command_Land::AMW_Command_Land() {
    this->commandStarted = false;
    this->completed = false;
}

void AMW_Command_Land::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->land();
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::getFacade()->sendDebug(PSTR("Starting landing..."));
        commandStarted = true;
    }
#endif
}

void AMW_Command_Land::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->isLanded()) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Landing Completed"));
#endif
    }
}
