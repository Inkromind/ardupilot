/*
 * AMW_Command_Nav_Altitude.cpp
 *
 *  Created on: 15-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Altitude.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Command_Nav_Altitude::AMW_Command_Nav_Altitude(float alt) {
    this->altitude = alt;
    this->commandStarted = false;
    this->completed = false;
}

void AMW_Command_Nav_Altitude::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::navigateToAltitude(altitude);
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::sendDebug(PSTR("Starting ascend/descend..."));
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug(PSTR("Ascend/Descend Completed"));
#endif
    }
}
