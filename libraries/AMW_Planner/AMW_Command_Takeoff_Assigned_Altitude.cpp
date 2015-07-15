/*
 * AMW_Command_Takeoff_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Planner.h"
#include <AC_Facade.h>
#include <AMW_Corridors.h>

void AMW_Command_Takeoff_Assigned_Altitude::run() {

    if (!commandStarted) {
        this->altitude = AMW_Corridor_Manager::getInstance()->getReservedAltitude(AMW_Planner::getModuleIdentifier());
        if (this->altitude == 0)
            return;
        commandStarted = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Starting takeoff to assigned altitude (%.2fm)"), altitude / 100);
#endif
    }


    AMW_Command_Takeoff::run();

}
