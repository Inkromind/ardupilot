/*
 * AMW_Command_Nav_Altitude.cpp
 *
 *  Created on: 15-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Altitude.h"
#include <AC_Facade.h>

AMW_Command_Nav_Altitude::AMW_Command_Nav_Altitude(float alt) {
    this->altitude = alt;
}

void AMW_Command_Nav_Altitude::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::navigateToAltitude(altitude);
}

void AMW_Command_Nav_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::altitudeReached(altitude)) {
        completed = true;
    }
}
