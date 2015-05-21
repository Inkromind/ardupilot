/*
 * AMW_Command_Nav_Altitude.cpp
 *
 *  Created on: 15-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Altitude.h"
#include <AC_Facade.h>

AMW_Command_Nav_Altitude::AMW_Command_Nav_Altitude(float alt) : AMW_Command() {
    this->altitude = alt;
}

void AMW_Command_Nav_Altitude::run() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->navigateToAltitude(altitude);
#ifdef AMW_COMMAND_DEBUG
    if (!commandStarted) {
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Starting ascend/descend to %.2fm"), altitude / 100);
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Ascend/Descend completed to %.2fm"), altitude / 100);
#endif
    }
}
