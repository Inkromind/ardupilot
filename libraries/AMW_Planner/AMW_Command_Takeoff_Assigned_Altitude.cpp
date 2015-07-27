/*
 * AMW_Command_Takeoff_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Planner.h"
#include <AC_CommunicationFacade.h>
#include <AMW_Corridors.h>

void AMW_Command_Takeoff_Assigned_Altitude::run(bool attempt) {

    if (completed)
        return;

    if (!commandStarted && !altitude) {
        altitude = AMW_Corridor_Manager::getInstance()->getReservedAltitude(AMW_Planner::getModuleIdentifier());
#ifdef AMW_COMMAND_DEBUG
        if (altitude)
            AC_CommunicationFacade::sendFormattedDebug(PSTR("Got assigned altitude (%.2fm)"), altitude / 1000);
#endif
    }

    AMW_Command_Takeoff::run(attempt);

}
