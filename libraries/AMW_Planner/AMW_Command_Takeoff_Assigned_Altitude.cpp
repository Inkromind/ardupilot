/*
 * AMW_Command_Takeoff_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Task_Planner.h"
#include <AC_Facade.h>

void AMW_Command_Takeoff_Assigned_Altitude::runCommand() {

    if (!commandStarted)
        this->altitude = AMW_Task_Planner::getInstance()->getAssignedAltitude();

    updateStatus();

    #ifdef AMW_PLANNER_DEBUG
        if (!commandStarted) {
            AC_Facade::getFacade()->sendDebug(PSTR("Starting takeoff..."));
            commandStarted = true;
        }
    #endif

    if (completed)
        return;

    AC_Facade::getFacade()->takeOff(altitude);

}
