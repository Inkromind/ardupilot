/*
 * AMW_Command_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff.h"
#include "AC_Facade.h"

AMW_Command_Takeoff::AMW_Command_Takeoff(float alt) {
    this->altitude = alt;
}

void AMW_Command_Takeoff::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::takeOff(altitude);
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::sendDebug("Starting takeoff...");
    }
    else {
        commandStarted = true;
    }
#endif
}

void AMW_Command_Takeoff::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug("Takeoff Completed");
#endif
    }
}
