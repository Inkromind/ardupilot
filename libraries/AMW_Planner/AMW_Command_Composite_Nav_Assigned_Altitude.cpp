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
#include <stdlib.h>
#include <AMW_Corridors.h>
#include "AMW_Planner.h"

#define AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS 200.0f

AMW_Command_Composite_Nav_Assigned_Altitude::AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f newDestination) : AMW_Command_Composite() {
    destination = Vector3f(newDestination.x, newDestination.y, 0);
    setNormalSubCommands();
}

void AMW_Command_Composite_Nav_Assigned_Altitude::completedSubCommand() {
    if ((currentState == RETURNTOSTART || currentState == LAND) && subCommands.empty()) {
        AMW_Corridor_Manager::getInstance()->clearReservedCorridors(AMW_Planner::getModuleIdentifier());
        setNormalSubCommands();
        return;
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::start() {
    if (currentState == NORMAL) {
        if (AMW_Corridor_Manager::getInstance()->hasReservedCorridors(AMW_Planner::getModuleIdentifier())) {
            commandStarted = true;
            return;
        }
        if (!reservingCorridors)
            reserveCorridors();
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::updateStatus() {
    if (completed)
        return;

    bool corridorConflict = checkCorridorConflict();

    if (currentState = NORMAL && corridorConflict) {
        returnToStart();
    }
    if (currentState = RETURNTOSTART && corridorConflict) {
        land();
    }
    markConflictResolved();

    if (AC_Facade::getFacade()->destinationReached(destination, AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS)) {
        completed = true;
#ifdef AMW_COMMAND_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Composite Nav Completed to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::setNormalSubCommands() {
    clearSubCommands();
    subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    subCommands.push(new AMW_Command_Nav_Assigned_Altitude(Vector2f(destination.x, destination.y)));
    subCommands.push(new AMW_Command_Land());
    Vector3f position = AC_Facade::getFacade()->getRealPosition();
    startLocation = Vector2f(position.x, position.y);
    currentState = NORMAL;
}

void AMW_Command_Composite_Nav_Assigned_Altitude::returnToStart() {
    clearSubCommands();
    subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    subCommands.push(new AMW_Command_Nav_Assigned_Altitude(start));
    subCommands.push(new AMW_Command_Land());
    currentState = RETURNTOSTART;
}

void AMW_Command_Composite_Nav_Assigned_Altitude::land() {
    clearSubCommands();
    clearReservedCorridors();
    reserveVerticalCorridor(currentLocation, currentAltitude, 0);
    subCommands.push(new AMW_Command_Land());
    subCommands.push(new AMW_Command_Delay(30 + rand() % 10));
    currentState = LAND;
}

