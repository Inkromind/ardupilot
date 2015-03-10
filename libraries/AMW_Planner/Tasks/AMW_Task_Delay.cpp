/*
 *AMW_Task_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Delay.h"
#include <AP_HAL.h>
#include <AP_Math.h>

AMW_Task_Delay::AMW_Task_Delay(uint32_t duration) {
    this->duration = (uint32_t)max(duration, 0);
    this->start = 0;
}

void AMW_Task_Delay::runTask() {
    if (running)
        return;

    if (completed)
        return;

    this->start = hal.scheduler->millis();
    this->running = true;
}

void AMW_Task_Delay::updateStatus() {
    if (completed)
           return;
    if (running)
           return;

    if (hal.scheduler->millis() - start > duration) {
        completed = true;
    }
}



