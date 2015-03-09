/*
 *AMW_Task_Delay.cpp
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Delay.h"
#include "AP_HAL.h"
#include "AP_Math.h"

AMW_Task_Delay::AMW_Task_Delay(uint32_t duration) {
    currentCommand = new AMW_Task_Command_Delay(duration);
}

void AMW_Task_Delay::runTask() {
    updateStatus();

    if (completed)
        return;

    currentCommand->runCommand();
}

void AMW_Task_Delay::updateStatus() {
    if (completed)
           return;

    currentCommand->updateStatus();

    this->completed = currentCommand->isComplete();
}



