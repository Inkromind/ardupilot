/*
 * AMW_Task_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Takeoff.h"
#include "AC_Facade.h"

AMW_Task_Takeoff::AMW_Task_Takeoff(float altitude) {
    this->altitude = altitude;
}

AMW_Task_Takeoff::~AMW_Task_Takeoff() {
}

void AMW_Task_Takeoff::runTask() {
    updateStatus();

    if (this->completed)
        return;

    AC_Facade::takeOff(altitude);
}

void AMW_Task_Takeoff::updateStatus() {
    if (this->completed)
        return;

    if (AC_Facade::getAltitude() >= this->altitude) {
        this->completed = true;
    }
}
