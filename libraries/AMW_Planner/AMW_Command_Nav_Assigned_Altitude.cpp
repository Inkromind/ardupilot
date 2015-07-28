/*
 * AMW_Command_Nav_Assigned_Altitude.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Nav_Assigned_Altitude.h"
#include <AC_Facade.h>
#include <AC_CommunicationFacade.h>
#include <AMW_Corridors.h>
#include "AMW_Planner.h"

void AMW_Command_Nav_Assigned_Altitude::run(bool attempt) {
    if (!commandStarted)
        this->destination.z = AMW_Corridor_Manager::getInstance()->getReservedAltitude(AMW_Planner::getModuleIdentifier());
    if (this->destination.z == 0)
        return;

    updateStatus();

    if (completed)
        return;

    bool oldStarted = commandStarted;
    commandStarted = AC_Facade::getFacade()->navigateTo(destination);
#ifdef AMW_COMMAND_DEBUG
    if (commandStarted && !oldStarted) {
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Starting nav to <%.0f,%.0f>"), destination.x / 100, destination.y / 100);
    }
#endif
}

void AMW_Command_Nav_Assigned_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Nav Completed to <%.0f,%.0f>"), destination.x / 100, destination.y / 100);
#endif
    }
}

