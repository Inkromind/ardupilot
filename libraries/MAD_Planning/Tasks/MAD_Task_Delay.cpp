/*
 * MAD_Task_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "MAD_Task_Delay.h"
#include "AP_HAL.h"
#include "AP_Math.h"

MAD_Task_Delay::MAD_Task_Delay(uint32_t duration) {
    this->duration = (uint32_t)max(duration, 0);
    this->start = 0;
}

MAD_Task_Delay::~MAD_Task_Delay() {
    // TODO Auto-generated destructor stub
}

void MAD_Task_Delay::runTask() {
    if (this->running)
        return;

    this->running = true;
    this->start = hal.scheduler->millis();
}

bool MAD_Task_Delay::isComplete() {
    if (this->completed)
        return true;
    if (!this->running)
        return false;

    if (hal.scheduler->millis() - this->start > this->duration) {
        this->completed = true;
        return true;
    }
    return false;
}

