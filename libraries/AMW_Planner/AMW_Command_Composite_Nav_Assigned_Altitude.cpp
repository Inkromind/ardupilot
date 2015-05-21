/*
 * AMW_Command_Composite_Nav_Assigned_Altitude.cpp
 *
 *  Created on: 21-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Composite_Nav_Assigned_Altitude.h"
#include "AMW_Command_Nav_Assigned_Altitude.h"
#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Command_Land.h"
#include <AC_Facade.h>

AMW_Command_Composite_Nav_Assigned_Altitude::AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f newDestination) : AMW_Command_Composite() {
    destination = Vector3f(newDestination.x, newDestination.y, 0);
    subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    subCommands.push(new AMW_Command_Nav_Assigned_Altitude(newDestination));
    subCommands.push(new AMW_Command_Land());
}

void AMW_Command_Composite_Nav_Assigned_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Composite Nav Completed to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
    }
}

