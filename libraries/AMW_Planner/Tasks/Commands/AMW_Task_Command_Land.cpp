/*
 * AMW_Task_Command_Land.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Command_Land.h"
#include "AC_Facade.h"

AMW_Task_Command_Land::AMW_Task_Command_Land() {
}

AMW_Task_Command_Land::~AMW_Task_Command_Land() {
}

void AMW_Task_Command_Land::runCommand() {
    updateStatus();

    if (this->completed)
        return;

    AC_Facade::land();
}

void AMW_Task_Command_Land::updateStatus() {
    if (this->completed)
        return;

    if (AC_Facade::isLanded()) {
        this->completed = true;
    }
}
