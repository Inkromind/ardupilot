/*
 * AMW_Command_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav.h"
#include <AC_Facade.h>

AMW_Command_Nav::AMW_Command_Nav(Vector3f waypoint) {
    this->destination = waypoint;
}

void AMW_Command_Nav::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::navigateTo(destination);
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::sendDebug(PSTR("Starting nav..."));
    }
    else {
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::destinationReached(destination)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug(PSTR("Nav Completed"));
#endif
    }
}
