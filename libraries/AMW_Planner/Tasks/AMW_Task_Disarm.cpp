/*
 * AMW_Task_Disarm.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Disarm.h"
#include <AC_Facade.h>

AMW_Task_Disarm::AMW_Task_Disarm() { }

void AMW_Task_Disarm::runTask() {
    if (completed)
        return;

    if (AC_Facade::disarmMotors())
        running = true;
}

void AMW_Task_Disarm::updateStatus() {
    if (completed)
        return;
    if (!running)
        return;

    if (!AC_Facade::areMotorsArmed()) {
        this->completed = true;
    }
}
