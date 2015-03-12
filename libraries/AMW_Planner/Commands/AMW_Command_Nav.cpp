/*
 * AMW_Command_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav.h"
#include <AC_Facade.h>

AMW_Command_Nav::AMW_Command_Nav(Vector3f destination) {
    this->destination = destination;
}

void AMW_Command_Nav::runCommand() {
    updateStatus();

    if (completed)
        return;

    AC_Facade::navigateTo(destination);
}

void AMW_Command_Nav::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::destinationReached(destination)) {
        this->completed = true;
    }
}
