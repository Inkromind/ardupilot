/*
 * AMW_Task_Takeoff.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Takeoff.h"
#include "AC_Facade.h"

AMW_Task_Takeoff::AMW_Task_Takeoff(float altitude) {
    currentCommand = new AMW_Task_Command_Takeoff(altitude);
}


void AMW_Task_Takeoff::runTask() {
    currentCommand->runCommand();
}

void AMW_Task_Takeoff::updateStatus() {
    if (this->completed)
        return;

    currentCommand->updateStatus();

    if (currentCommand->isComplete())
        this->completed = true;
}
