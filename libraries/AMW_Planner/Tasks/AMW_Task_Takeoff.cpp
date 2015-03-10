/*
 * AMW_Task_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Takeoff.h"
#include <AC_Facade.h>

AMW_Task_Takeoff::AMW_Task_Takeoff(float altitude) {
    this->altitude = altitude;
}

void AMW_Task_Takeoff::runTask() {
    if (completed)
        return;

    if (AC_Facade::takeOff(altitude))
        running = true;
}

void AMW_Task_Takeoff::updateStatus() {
    if (completed)
        return;

    if (!running)
        return;

    if (AC_Facade::takeOffComplete(altitude)) {
        completed = true;
    }
}
