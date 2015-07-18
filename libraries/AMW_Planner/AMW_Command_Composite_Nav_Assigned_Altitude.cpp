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
#include "AMW_Planner.h"
#include "AMW_Task_Planner.h"

#define AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS 200.0f
#define AMW_COMMAND_BATTERY_TAKEOFF_LIMIT 40

AMW_Command_Composite_Nav_Assigned_Altitude::AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f newDestination) : AMW_Command_Composite() {
    destination = Vector3f(newDestination.x, newDestination.y, 0);
    setNormalSubCommands();
}

void AMW_Command_Composite_Nav_Assigned_Altitude::completedSubCommand() {
    if (subCommands.empty()) {
        clearReservedCorridors();
    }
    if ((currentState == RETURNTOSTART || currentState == LAND) && subCommands.empty()) {
        setNormalSubCommands();
        return;
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::start() {
    if (currentState == NORMAL) {
        if (AC_Facade::getFacade()->getBattery()->capacity_remaining_pct() < AMW_COMMAND_BATTERY_TAKEOFF_LIMIT) {
            AMW_Task_Planner::getInstance()->markBatteryEmpty();
            return;
        }
        if (corridors.empty()) {
            reserveNormalCorridors();
        }
        else if (AMW_Corridor_Manager::getInstance()->corridorsAreReserved(&corridors)) {
            commandStarted = true;
        }
        else if (AMW_Corridor_Manager::getInstance()->reservationHasFailed(AMW_Planner::getModuleIdentifier())) {
            //failed = true;
            //TODO
        }
        else if (!AMW_Corridor_Manager::getInstance()->isReservingCorridors(AMW_Planner::getModuleIdentifier())) {
            AMW_Corridor_Manager::getInstance()->reserveCorridors(AMW_Planner::getModuleIdentifier(), &corridors);
        }
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::updateStatus() {
    if (completed)
        return;

    if (AC_Facade::getFacade()->destinationReached(destination, AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS)) {
            completed = true;
            clearReservedCorridors();
            AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
#ifdef AMW_COMMAND_DEBUG
            AC_Facade::getFacade()->sendFormattedDebug(PSTR("Composite Nav Completed to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
            return;
#endif
    }

    if (!commandStarted)
        return;

    if (!AMW_Corridor_Manager::getInstance()->corridorsAreReserved(&corridors)) {
        commandStarted = false;
        return;
    }

    bool corridorConflict = AMW_Corridor_Manager::getInstance()->hasCorridorConflict(AMW_Planner::getModuleIdentifier());

    if (corridorConflict) {
        if (currentState == NORMAL && corridorConflict) {
            returnToStart();
        }
        if (currentState == RETURNTOSTART && corridorConflict) {
            land();
        }
        AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
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
    Vector3f position = AC_Facade::getFacade()->getRealPosition();
    corridors.push_back(new AMW_Vertical_Corridor(Vector2f(position.x, position.y), position.z));
    AMW_Corridor_Manager::getInstance()->markCorridorsReserved(AMW_Planner::getModuleIdentifier(), &corridors);
    subCommands.push(new AMW_Command_Land());
    subCommands.push(new AMW_Command_Delay(30 + rand() % 10));
    currentState = LAND;
}

void AMW_Command_Composite_Nav_Assigned_Altitude::clearReservedCorridors() {
    AMW_Corridor_Manager::getInstance()->freeCorridors(&corridors);
    corridors.clear();
}

void AMW_Command_Composite_Nav_Assigned_Altitude::reserveNormalCorridors() {
    if (!corridors.empty()) {
        clearReservedCorridors();
    }
    corridors.push_back(new AMW_Vertical_Corridor(startLocation));
    corridors.push_back(new AMW_Horizontal_Corridor(startLocation, Vector2f(destination.x, destination.y)));
    corridors.push_back(new AMW_Vertical_Corridor(Vector2f(destination.x, destination.y)));
}

