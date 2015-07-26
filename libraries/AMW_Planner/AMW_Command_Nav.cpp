/*
 * AMW_Command_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav.h"
#include <AC_Facade.h>

AMW_Command_Nav::AMW_Command_Nav(Vector3f waypoint) : AMW_Command() {
    this->destination = waypoint;
}

void AMW_Command_Nav::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->navigateTo(destination);
#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Starting nav to <%.2f,%.2f> | Altitude: %.2fm"), destination.x / 100, destination.y / 100, destination.z / 100);
    }
#endif
}

void AMW_Command_Nav::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Nav Completed to <%.2f,%.2f> | Altitude: %.2fm"), destination.x / 100, destination.y / 100, destination.z / 100);
#endif
    }
}
