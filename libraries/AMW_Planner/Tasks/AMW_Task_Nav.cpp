/*
 * AMW_Task_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Nav.h"
#include "AC_Facade.h"

AMW_Task_Nav::AMW_Task_Nav(Vector3f destination) {
    currentCommand = new AMW_Task_Command_Nav(destination);
}

void AMW_Task_Nav::runTask() {
    updateStatus();

    if (completed)
        return;

    currentCommand->runCommand();
}

void AMW_Task_Nav::updateStatus() {
    if (completed)
        return;

    currentCommand->updateStatus();

    completed = currentCommand->isComplete();
}
