/*
 * AMW_Command_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Command_Takeoff::AMW_Command_Takeoff(float alt) {
    this->altitude = alt;
    this->commandStarted = false;
    this->completed = false;
}

void AMW_Command_Takeoff::runCommand() {
    updateStatus();

#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::sendDebug(PSTR("Starting takeoff..."));
        commandStarted = true;
    }
#endif

    if (completed)
        return;

    AC_Facade::takeOff(altitude);
}

void AMW_Command_Takeoff::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug(PSTR("Takeoff Completed"));
#endif
    }
}
