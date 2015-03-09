/*
 * AMW_Task_Command_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Command_Nav.h"
#include "AC_Facade.h"

AMW_Task_Command_Nav::AMW_Task_Command_Nav(Vector3f destination) {
    this->destination = destination;
}

AMW_Task_Command_Nav::~AMW_Task_Command_Nav() {
    delete destination;
}

void AMW_Task_Command_Nav::runCommand() {
    updateStatus();

    if (this->completed)
        return;

    AC_Facade::navigateTo(this->destination);
}

void AMW_Task_Command_Nav::updateStatus() {
    if (this->completed)
        return;
    if (!this->running)
        return;

    if (AC_Facade::destinationReached(this->destination)) {
        this->completed = true;
    }
}
