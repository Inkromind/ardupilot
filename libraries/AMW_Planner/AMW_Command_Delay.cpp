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
#include "AMW_Planner.h"

extern const AP_HAL::HAL& hal;

AMW_Command_Delay::AMW_Command_Delay(uint32_t delay) {
    this->duration = (uint32_t)max(delay, 0);
    this->start = 0;
    this->completed = false;
    this->commandStarted = false;
}

void AMW_Command_Delay::runCommand() {
    updateStatus();

    AC_Facade::getFacade()->loiter();

    if (commandStarted)
        return;

    if (completed)
        return;

    this->start = hal.scheduler->millis();
    this->commandStarted = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Starting delay"));
#endif
}

void AMW_Command_Delay::updateStatus() {
    if (completed)
           return;
    if (!commandStarted)
           return;

    if (hal.scheduler->millis() - start > duration) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Delay Completed"));
#endif
    }
}

void AMW_Command_Delay::pauseCommand() {
    this->duration -= hal.scheduler->millis() - start;
    start = 0;
}

void AMW_Command_Delay::resumeCommand() {
    this->start = hal.scheduler->millis();
}
