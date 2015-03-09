/*
 *AMW_Task_Command_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Command_Delay.h"
#include "AP_HAL.h"
#include "AP_Math.h"

AMW_Task_Command_Delay::AMW_Task_Command_Delay(uint32_t duration) {
    this->duration = (uint32_t)max(duration, 0);
    this->start = 0;
}

void AMW_Task_Command_Delay::runCommand() {
    if (this->running)
        return;

    this->running = true;
    this->start = hal.scheduler->millis();
}

void AMW_Task_Command_Delay::updateStatus() {
    if (this->completed)
           return;
    if (!this->running)
           return;

    if (hal.scheduler->millis() - this->start > this->duration) {
        this->completed = true;
    }
}



