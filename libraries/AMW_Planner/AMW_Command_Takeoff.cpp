/*
 * AMW_Command_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff.h"
#include <AC_Facade.h>

AMW_Command_Takeoff::AMW_Command_Takeoff(float alt) : AMW_Command() {
    this->altitude = alt;
}

void AMW_Command_Takeoff::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->takeOff(altitude);

#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Starting takeoff to %.2fm"), altitude / 100);
        commandStarted = true;
    }
#endif
}

void AMW_Command_Takeoff::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Takeoff Completed to %.2fm"), altitude / 100);
#endif
    }
}
