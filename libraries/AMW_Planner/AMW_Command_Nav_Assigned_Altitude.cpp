/*
 * AMW_Command_Nav_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Assigned_Altitude.h"
#include "AMW_Task_Planner.h"
#include <AC_Facade.h>

void AMW_Command_Nav_Assigned_Altitude::run() {
    if (!commandStarted)
        this->destination.z = AMW_Task_Planner::getInstance()->getAssignedAltitude();

    updateStatus();

    if (completed)
        return;

    AC_Facade::getFacade()->navigateTo(destination);
#ifdef AMW_COMMAND_DEBUG
    if (!commandStarted) {
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Starting nav to <%.0f,%.0f>"), destination.x / 100, destination.y / 100);
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav_Assigned_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Nav Completed to <%.0f,%.0f>"), destination.x / 100, destination.y / 100);
#endif
    }
}

