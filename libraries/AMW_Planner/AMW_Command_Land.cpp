/*
 * AMW_Command_Land.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Land.h"
#include <AC_Facade.h>

AMW_Command_Land::AMW_Command_Land() : AMW_Command() { }

void AMW_Command_Land::run() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->land();
#ifdef AMW_COMMAND_DEBUG
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
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Landing Completed"));
#endif
    }
}
