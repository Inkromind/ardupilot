/*
 *AMW_Command_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Delay.h"
#include <AP_HAL.h>
#include <AP_Math.h>

AMW_Command_Delay::AMW_Command_Delay(uint32_t duration) {
    this->duration = (uint32_t)max(duration, 0);
    this->start = 0;
}

void AMW_Command_Delay::runCommand() {
    updateStatus();

    if (running)
        return;

    if (completed)
        return;

    this->start = hal.scheduler->millis();
    this->running = true;
}

void AMW_Command_Delay::updateStatus() {
    if (completed)
           return;
    if (running)
           return;

    if (hal.scheduler->millis() - start > duration) {
        completed = true;
    }
}



