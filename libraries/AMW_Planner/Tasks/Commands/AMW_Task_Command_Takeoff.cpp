/*
 * AMW_Task_Command_Takeoff.cpp
 *
 *  Created on: 9-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Command_Takeoff.h"

AMW_Task_Command_Takeoff::~AMW_Task_Command_Takeoff() {
}

AMW_Task_Command_Takeoff::AMW_Task_Command_Takeoff(float altitude) {
    this->altitude = altitude;
}

void AMW_Task_Command_Takeoff::runCommand() {
    updateStatus();

    if (this->completed)
        return;

    AC_Facade::takeOff(altitude);
}

void AMW_Task_Command_Takeoff::updateStatus() {
    if (this->completed)
        return;

    if (AC_Facade::takeOffComplete(altitude)) {
        this->completed = true;
    }
}
