/*
 * AMW_Task_Nav.cpp
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#include "AMW_Task_Nav.h"
#include <AC_Facade.h>

AMW_Task_Nav::AMW_Task_Nav(Vector3f destination) {
    this->destination = destination;
}

void AMW_Task_Nav::runTask() {
    if (completed)
        return;

    if (AC_Facade::navigateTo(destination))
        running = true;
}

void AMW_Task_Nav::updateStatus() {
    if (completed)
        return;
    if (!running)
        return;

    if (AC_Facade::destinationReached(destination)) {
        this->completed = true;
    }
}
