/*
 *AMW_Command_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Delay.h"
#include <AP_HAL.h>
#include <AP_Math.h>
#include <AC_Facade.h>

extern const AP_HAL::HAL& hal;

AMW_Command_Delay::AMW_Command_Delay(uint32_t delay) {
    this->duration = (uint32_t)max(delay, 0);
    this->start = 0;
}

void AMW_Command_Delay::runCommand() {
    updateStatus();

    if (commandStarted)
        return;

    if (completed)
        return;

    this->start = hal.scheduler->millis();
    this->commandStarted = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::sendDebug(PSTR("Starting delay"));
#endif
}

void AMW_Command_Delay::updateStatus() {
    if (completed)
           return;
    if (commandStarted)
           return;

    if (hal.scheduler->millis() - start > duration) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::sendDebug(PSTR("Delay Completed"));
#endif
    }
}



