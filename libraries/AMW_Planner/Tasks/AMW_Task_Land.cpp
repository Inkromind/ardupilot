/*
 * AMW_Task_Land.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Land.h"
#include "AC_Facade.h"

AMW_Task_Land::AMW_Task_Land() {
}

void AMW_Task_Land::runTask() {
    updateStatus();

    if (completed)
        return;

    if (AC_Facade::land())
        running = true;
}

void AMW_Task_Land::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::isLanded()) {
        this->completed = true;
    }
}
