/*
 * AMW_Command_Nav_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Assigned_Altitude.h"

void AMW_Command_Nav_Assigned_Altitude::runCommand() {
    if (!commandStarted)
        this->destination.z = AMW_Task_Planner::getInstance()->getAssignedAltitude();

    updateStatus();

    if (completed)
        return;



    AC_Facade::getFacade()->navigateTo(destination);
#ifdef AMW_PLANNER_DEBUG
    if (!commandStarted) {
        AC_Facade::getFacade()->sendDebug(PSTR("Starting nav..."));
        commandStarted = true;
    }
#endif
}

void AMW_Command_Nav::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Nav Completed"));
#endif
    }
}

