/*
 * AMW_Command_Disarm.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Disarm.h"
#include <AC_Facade.h>

AMW_Command_Disarm::AMW_Command_Disarm() : AMW_Command() { }

void AMW_Command_Disarm::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->disarmMotors();
#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_Facade::getFacade()->sendDebug(PSTR("Starting disarm..."));
    }
#endif

    updateStatus();
}

void AMW_Command_Disarm::updateStatus() {
    if (completed)
        return;

    if (!AC_Facade::getFacade()->areMotorsArmed()) {
        this->completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Disarming Completed"));
#endif
    }
}
