/*
 * AMW_Command_Land.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Land.h"
#include <AC_Facade.h>

AMW_Command_Land::AMW_Command_Land() : AMW_Command() { }

void AMW_Command_Land::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->land();
#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_Facade::getFacade()->sendDebug(PSTR("Starting landing..."));
    }
#endif
}

void AMW_Command_Land::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->isLanded()) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Landing Completed"));
#endif
    }
}
