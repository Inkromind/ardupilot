/*
 * AMW_Command_Disarm.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Disarm.h"
#include <AC_Facade.h>

AMW_Command_Disarm::AMW_Command_Disarm() { }

void AMW_Command_Disarm::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::disarmMotors();
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::sendDebug("Starting disarm...");
    }
    else {
        commandStarted = true;
    }
#endif

    updateStatus();
}

void AMW_Command_Disarm::updateStatus() {
    if (completed)
        return;

    if (!AC_Facade::areMotorsArmed()) {
        this->completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug("Disarming Completed");
#endif
    }
}
