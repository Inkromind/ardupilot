/*
 * MAD_Task_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Takeoff.h"
#include "AC_Facade.h"

MAD_Task_Takeoff::MAD_Task_Takeoff(float altitude) {
    this->altitude = altitude;
}

MAD_Task_Takeoff::~MAD_Task_Takeoff() {
}

void MAD_Task_Takeoff::runTask() {
    if (this->running)
        return;

    if (AC_Facade::startTakeOff(this->altitude))
        this->running = true;
}

bool MAD_Task_Takeoff::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    if (AC_Facade::getAltitude() >= this->altitude) {
        this->completed = true;
        return true;
    }
    return false;
}
