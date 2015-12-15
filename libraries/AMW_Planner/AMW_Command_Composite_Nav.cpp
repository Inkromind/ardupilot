/*
 * AMW_Command_Composite_Nav_Assigned_Altitude.cpp
 *
 *  Created on: 21-mei-2015
 *      Author: Arne
 */

#include "AMW_Command_Composite_Nav.h"
#include "AMW_Command_Nav_Assigned_Altitude.h"
#include "AMW_Command_Nav_Altitude.h"
#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Command_Land.h"
#include "AMW_Command_Delay.h"
#include <AC_Facade.h>
#include <AC_CommunicationFacade.h>
#include <stdlib.h>
#include "AMW_Planner.h"
#include "AMW_Task_Planner.h"
#include <stdint.h>
#include "AMW_Sequencer.h"

AMW_Command_Composite_Nav::AMW_Command_Composite_Nav(Vector2f newDestination, bool returnHome) : AMW_Command_Composite() {
    destination = Vector3f(newDestination.x, newDestination.y, 0);
    currentState = INIT;
#ifdef AMW_COMMAND_ONLY_LAND_AT_BASE
    this->returningHome = returnHome;
#else
    this->returningHome = true;
#endif
    setNormalSubCommands();
}

void AMW_Command_Composite_Nav::completedSubCommand() {
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
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Returned to start. Retrying nav"));
#endif
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
            clearReservedCorridors();
        }
    }
    else if (currentState == LAND) {
        if (subCommands.empty()) {
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Landed. Retrying nav"));
#endif
            setNormalSubCommands();
            commandStarted = false;
            currentState = INIT;
        }
        else if (subCommands.size() == 1) {
            clearReservedCorridors();
        }
    }
}

void AMW_Command_Composite_Nav::startCommand(bool attempt) {
    if (currentState == INIT) {
        if (AC_Facade::getFacade()->getBattery()->capacity_remaining_pct() < AMW_PLANNER_BATTERY_TAKEOFF_LIMIT) {
            AMW_Task_Planner::getInstance()->markBatteryEmpty();
            failed = true;
            return;
        }
        Vector3f position = AC_Facade::getFacade()->getRealPosition();
        startLocation = Vector2f(position.x, position.y);
        getCorridors();
        bool reserving = false;
        if (attempt)
            reserving = AMW_Corridor_Manager::getInstance()->reserveCorridors(AMW_Planner::getModuleIdentifier(), &corridors, 1, AC_Facade::getFacade()->getAltitude());
        else
            reserving = AMW_Corridor_Manager::getInstance()->reserveCorridors(AMW_Planner::getModuleIdentifier(), &corridors);
        if (!reserving) {
            failed = true;
            currentState = FAILED;
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Unable to reserve corridors for nav"));
#endif
        }
        else {
            currentState = WAITING_FOR_CORRIDORS;
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Attempting to reserve corridors for nav"));
#endif
        }
    }
}

void AMW_Command_Composite_Nav::updateStatus() {
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
            AC_CommunicationFacade::sendFormattedDebug(PSTR("Already at <%.2f,%.2f>. Skipping Nav"), destination.x / 100, destination.y / 100);
#endif
            return;
    }

    bool corridorConflict = AMW_Corridor_Manager::getInstance()->hasCorridorConflict(AMW_Planner::getModuleIdentifier());

    if (corridorConflict) {
        AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
        if (currentState == NORMAL && corridorConflict) {
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Corridor conflict detected. Returning to start"));
#endif
            returnToStart();
            updateStatus();
            return;
        }
        if (currentState == RETURNTOSTART && corridorConflict) {
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Corridor conflict detected. Landing"));
#endif
            land();
            updateStatus();
            return;
        }
    }

    if (currentState != INIT) {

        bool corridorsAreReserved = false;
        if (!corridors.empty())
            corridorsAreReserved = AMW_Corridor_Manager::getInstance()->corridorsAreReserved(AMW_Planner::getModuleIdentifier(), &corridors);

        if (currentState == WAITING_FOR_CORRIDORS) {
            if (corridorsAreReserved) {
                commandStarted = true;
                currentState = NORMAL;
                corridors.get(0)->setInCorridor(true);
#ifdef AMW_COMMAND_DEBUG
                AC_CommunicationFacade::sendFormattedDebug(PSTR("Corridors Reserved. Starting nav to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
            }
            else if (AMW_Corridor_Manager::getInstance()->reservationHasFailed(AMW_Planner::getModuleIdentifier())) {
                failed = true;
                currentState = FAILED;
                clearReservedCorridors();
#ifdef AMW_COMMAND_DEBUG
                AC_CommunicationFacade::sendFormattedDebug(PSTR("Corridor reservation failed. Nav to <%.2f,%.2f> failed"), destination.x / 100, destination.y / 100);
#endif
            }
            else if (!AMW_Corridor_Manager::getInstance()->isReservingCorridors(AMW_Planner::getModuleIdentifier())) {
                if (!AMW_Corridor_Manager::getInstance()->reserveCorridors(AMW_Planner::getModuleIdentifier(), &corridors)) {
                    failed = true;
                    currentState = FAILED;
                    clearReservedCorridors();
#ifdef AMW_COMMAND_DEBUG
                    AC_CommunicationFacade::sendFormattedDebug(PSTR("Unable to restart corridor reservation for nav to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
                }
                else {
#ifdef AMW_COMMAND_DEBUG
                    AC_CommunicationFacade::sendFormattedDebug(PSTR("Restarted corridor reservation for nav to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
                }
            }
        }
        else if (currentState == NORMAL) {
            if (!corridorsAreReserved) {
#ifdef AMW_COMMAND_DEBUG
                AC_CommunicationFacade::sendDebug(PSTR("Error. Corridors no longer reserved. Landing"));
#endif
                land();
            }
        }
        else if (currentState == RETURNTOSTART) {
            if (!corridorsAreReserved && !corridors.empty()) {
                land();
#ifdef AMW_COMMAND_DEBUG
                AC_CommunicationFacade::sendDebug(PSTR("Error. Corridors no longer reserved. Landing"));
#endif
            }
        }
        else if (currentState == LAND) {
            if (!corridorsAreReserved && !corridors.empty())
                AMW_Corridor_Manager::getInstance()->markCorridorsReserved(AMW_Planner::getModuleIdentifier(), &corridors);
        }
    }

}

void AMW_Command_Composite_Nav::setNormalSubCommands() {
    clearSubCommands();
    subCommands.push(new AMW_Command_Takeoff_Assigned_Altitude());
    subCommands.push(new AMW_Command_Nav_Assigned_Altitude(Vector2f(destination.x, destination.y)));
    if (returningHome)
        subCommands.push(new AMW_Command_Land());
    else
        subCommands.push(new AMW_Command_Nav_Altitude(AMW_COMMAND_HOVER_ALTITUDE));
}

void AMW_Command_Composite_Nav::returnToStart() {
    AMW_Sequencer::getInstance()->totalReturnToStarts++;
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
#ifdef AMW_COMMAND_ONLY_LAND_AT_BASE
    subCommands.push(new AMW_Command_Nav_Altitude(AMW_COMMAND_HOVER_ALTITUDE));
#else
    subCommands.push(new AMW_Command_Land());
#endif
    subCommands.push(new AMW_Command_Delay((10 + rand() % 5) * 1000));

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

void AMW_Command_Composite_Nav::land() {
    AMW_Sequencer::getInstance()->totalEmergencyLandings++;
    clearSubCommands();
    clearReservedCorridors();
    Vector3f position = AC_Facade::getFacade()->getRealPosition();
    corridors.push_back(new AMW_Vertical_Corridor(Vector2f(position.x, position.y), position.z));
    AMW_Corridor_Manager::getInstance()->markCorridorsReserved(AMW_Planner::getModuleIdentifier(), &corridors);
#ifdef AMW_COMMAND_ONLY_LAND_AT_BASE
    subCommands.push(new AMW_Command_Nav_Altitude(AMW_COMMAND_HOVER_ALTITUDE));
#else
    subCommands.push(new AMW_Command_Land());
#endif
    subCommands.push(new AMW_Command_Delay((10 + rand() % 5) * 1000));
    currentState = LAND;
}

void AMW_Command_Composite_Nav::clearReservedCorridors() {
    if (!corridors.empty()) {
        AMW_Corridor_Manager::getInstance()->freeCorridors(&corridors);
        corridors.clear();
    }
}

void AMW_Command_Composite_Nav::getCorridors() {
    if (!corridors.empty()) {
        clearReservedCorridors();
    }
    corridors.push_back(new AMW_Vertical_Corridor(startLocation));
    corridors.push_back(new AMW_Horizontal_Corridor(startLocation, Vector2f(destination.x, destination.y)));
    corridors.push_back(new AMW_Vertical_Corridor(Vector2f(destination.x, destination.y)));
}

