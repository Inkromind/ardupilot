/*
 * AMW_Task_Land_Command.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Land_Command.h"
#include <AC_Facade.h>

AMW_Task_Land_Command::AMW_Task_Land_Command() { }

void AMW_Task_Land_Command::runTask() {
    if (completed)
        return;

    if (AC_Facade::land())
        running = true;
}

void AMW_Task_Land_Command::updateStatus() {
    if (completed)
        return;

    if (!running)
        return;

    if (AC_Facade::isLanded()) {
        completed = true;
    }
}
