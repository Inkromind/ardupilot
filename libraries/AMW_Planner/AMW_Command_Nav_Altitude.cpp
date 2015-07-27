/*
 * AMW_Command_Nav_Altitude.cpp
 *
 *  Created on: 15-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Altitude.h"
#include <AC_Facade.h>
#include <AC_CommunicationFacade.h>

AMW_Command_Nav_Altitude::AMW_Command_Nav_Altitude(float alt) : AMW_Command() {
    this->altitude = alt;
}

void AMW_Command_Nav_Altitude::run(bool attempt) {
    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->navigateToAltitude(altitude);
#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Starting ascend/descend to %.2fm"), altitude / 100);
    }
#endif
}

void AMW_Command_Nav_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->altitudeReached(altitude)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Ascend/Descend completed to %.2fm"), altitude / 100);
#endif
    }
}
