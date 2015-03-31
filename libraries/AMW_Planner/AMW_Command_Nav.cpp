/*
 * AMW_Command_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Command_Nav::AMW_Command_Nav(Vector3f waypoint) {
    this->destination = waypoint;
    this->commandStarted = false;
    this->completed = false;
}

void AMW_Command_Nav::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->navigateTo(destination);
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::getFacade()->sendDebug(PSTR("Starting nav..."));
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Nav Completed"));
#endif
    }
}
