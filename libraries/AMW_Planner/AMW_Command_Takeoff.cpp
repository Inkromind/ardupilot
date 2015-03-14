/*
 * AMW_Command_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff.h"
#include "AC_Facade.h"

AMW_Command_Takeoff::AMW_Command_Takeoff(float targetAltitude) {
    this->altitude = targetAltitude;
}

void AMW_Command_Takeoff::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::takeOff(altitude);
}

void AMW_Command_Takeoff::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::takeOffComplete(altitude)) {
        completed = true;
    }
}
