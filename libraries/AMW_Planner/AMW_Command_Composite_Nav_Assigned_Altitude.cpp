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
#include "AMW_Command_Delay.h"
#include <AC_Facade.h>
#include <AC_CommunicationFacade.h>
#include <stdlib.h>
#include "AMW_Planner.h"
#include "AMW_Task_Planner.h"
#include <stdint.h>

AMW_Command_Composite_Nav_Assigned_Altitude::AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f newDestination) : AMW_Command_Composite() {
    destination = Vector3f(newDestination.x, newDestination.y, 0);
    currentState = INIT;
    setNormalSubCommands();
}

void AMW_Command_Composite_Nav_Assigned_Altitude::completedSubCommand() {
    if (subCommands.empty()) {
        clearReservedCorridors();
    }
    if (currentState == NORMAL) {
        if (subCommands.size() == 2) {
            corridors.get(0)->setCompleted(true); // Vertical 1
            corridors.get(0)->setInCorridor(false);
            corridors.get(1)->setInCorridor(true); // Horizontal
        }
        else if (subCommands.size() == 1) {
            corridors.get(1)->setCompleted(true); // Horizontal
            corridors.get(1)->setInCorridor(false);
            corridors.get(2)->setInCorridor(true); // Vertical 2
        }
    }
    else if (currentState == RETURNTOSTART) {
        if (subCommands.empty()) {
            setNormalSubCommands();
            commandStarted = false;
            currentState = INIT;
        }
        else if (subCommands.size() == 3) {
            corridors.get(2)->setCompleted(true); // Vertical 2
            corridors.get(2)->setInCorridor(false);
            corridors.get(1)->setInCorridor(true); // Horizontal
        }
        else if (subCommands.size() == 2) {
            corridors.get(1)->setCompleted(true); // Horizontal
            corridors.get(1)->setInCorridor(false);
            corridors.get(0)->setInCorridor(true); // Vertical 1
        }
        else if (subCommands.size() == 1) {
            corridors.get(0)->setCompleted(true); // Horizontal
            corridors.get(0)->setInCorridor(false);
            clearReservedCorridors();
        }
    }
    else if (currentState == LAND) {
        if (subCommands.empty()) {
            setNormalSubCommands();
            commandStarted = false;
            currentState = INIT;
        }
        else if (subCommands.size() == 1) {
            clearReservedCorridors();
        }
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::startCommand(bool attempt) {
    if (currentState == INIT) {
        if (AC_Facade::getFacade()->getBattery()->capacity_remaining_pct() < AMW_COMMAND_BATTERY_TAKEOFF_LIMIT) {
            AMW_Task_Planner::getInstance()->markBatteryEmpty();
            failed = true;
            return;
        }
        Vector3f position = AC_Facade::getFacade()->getRealPosition();
        startLocation = Vector2f(position.x, position.y);
        getCorridors();
        bool reserving = false;
        if (attempt)
            reserving = AMW_Corridor_Manager::getInstance()->reserveCorridors(0, &corridors, 1);
        else
            reserving = AMW_Corridor_Manager::getInstance()->reserveCorridors(0, &corridors);
        if (!reserving) {
            failed = true;
            currentState = FAILED;
        }
        else {
            currentState = WAITING_FOR_CORRIDORS;
        }
    }
}



void AMW_Command_Composite_Nav_Assigned_Altitude::updateStatus() {
    if (completed || failed)
        return;

    if (subCommands.empty()) {
        completed = true;
        currentState = COMPLETED;
        return;
    }

    if (currentState == INIT && AC_Facade::getFacade()->destinationReached(destination, AMW_COMMAND_COMPOSITE_NAV_DESTINATION_RADIUS)) {
            completed = true;
            currentState = COMPLETED;
            clearReservedCorridors();
            AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendFormattedDebug(PSTR("Composite Nav Completed to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
            return;
    }

    bool corridorConflict = AMW_Corridor_Manager::getInstance()->hasCorridorConflict(AMW_Planner::getModuleIdentifier());

    if (corridorConflict) {
        AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
        if (currentState == NORMAL && corridorConflict) {
            returnToStart();
            return;
        }
        if (currentState == RETURNTOSTART && corridorConflict) {
            land();
            return;
        }
    }

    if (currentState != INIT) {

        bool isReservingCorridors = AMW_Corridor_Manager::getInstance()->isReservingCorridors(AMW_Planner::getModuleIdentifier());
        bool corridorsAreReserved = false;
        if (!corridors.empty())
            corridorsAreReserved = AMW_Corridor_Manager::getInstance()->corridorsAreReserved(AMW_Planner::getModuleIdentifier(), &corridors);

        if (currentState == WAITING_FOR_CORRIDORS) {
            if (corridorsAreReserved) {
                commandStarted = true;
                currentState = NORMAL;
                corridors.get(0)->setInCorridor(true);
            }
            else if (AMW_Corridor_Manager::getInstance()->reservationHasFailed(AMW_Planner::getModuleIdentifier())) {
                failed = true;
                currentState = FAILED;
            }
            else if (!isReservingCorridors) {
                if (!AMW_Corridor_Manager::getInstance()->reserveCorridors(AMW_Planner::getModuleIdentifier(), &corridors)) {
                    failed = true;
                    currentState = FAILED;
                }
            }
        }
        else if (currentState == NORMAL) {
            if (!corridorsAreReserved)
                land();
        }
        else if (currentState == RETURNTOSTART) {
            if (!corridorsAreReserved && !corridors.empty())
                land();
        }
        else if (currentState == LAND) {
            if (!corridorsAreReserved && !corridors.empty())
                AMW_Corridor_Manager::getInstance()->markCorridorsReserved(AMW_Planner::getModuleIdentifier(), &corridors);
        }
    }

}

void AMW_Command_Composite_Nav_Assigned_Altitude::setNormalSubCommands() {
    clearSubCommands();
    subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    subCommands.push(new AMW_Command_Nav_Assigned_Altitude(Vector2f(destination.x, destination.y)));
    subCommands.push(new AMW_Command_Land());
}

void AMW_Command_Composite_Nav_Assigned_Altitude::returnToStart() {
    uint32_t remainingCommands = subCommands.size();
    clearSubCommands();

    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors.iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setReverseDirection(true);
        corridor->setCompleted(false);
        corridor->setInCorridor(false);
    }
    delete iterator;

    if (remainingCommands <= 1)
        subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    if (remainingCommands <= 2)
        subCommands.push(new AMW_Command_Nav_Assigned_Altitude(startLocation));
    subCommands.push(new AMW_Command_Land());
    subCommands.push(new AMW_Command_Delay(30 + rand() % 10));

    if (subCommands.size() == 4) {
        corridors.get(2)->setInCorridor(true);
    }
    else if (subCommands.size() == 3) {
        corridors.get(2)->setCompleted(true);
        corridors.get(1)->setInCorridor(true);
    }
    else {
        corridors.get(2)->setCompleted(true);
        corridors.get(1)->setCompleted(true);
        corridors.get(0)->setInCorridor(true);
    }
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
    if (!corridors.empty()) {
        AMW_Corridor_Manager::getInstance()->freeCorridors(&corridors);
        corridors.clear();
    }
}

void AMW_Command_Composite_Nav_Assigned_Altitude::getCorridors() {
    if (!corridors.empty()) {
        clearReservedCorridors();
    }
    corridors.push_back(new AMW_Vertical_Corridor(startLocation));
    corridors.push_back(new AMW_Horizontal_Corridor(startLocation, Vector2f(destination.x, destination.y)));
    corridors.push_back(new AMW_Vertical_Corridor(Vector2f(destination.x, destination.y)));
}

