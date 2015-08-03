/*
 * AMW_Command_Composite_Nav_Assigned_Altitude_Forced.cpp
 *
 *  Created on: 2-aug.-2015
 *      Author: Arne
 */

#include "AMW_Command_Composite_Forced_Nav.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"
#include <AC_CommunicationFacade.h>

void AMW_Command_Composite_Forced_Nav::startCommand(bool attempt) {
    if (currentState == INIT) {
        Vector3f position = AC_Facade::getFacade()->getRealPosition();
        startLocation = Vector2f(position.x, position.y);
        getCorridors();
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors.iterator();
        while (iterator->hasNext()) {
            iterator->next()->setAltitude(1500);
        }
        delete iterator;
        AMW_Corridor_Manager::getInstance()->markCorridorsReserved(AMW_Planner::getModuleIdentifier(), &corridors);
        currentState = NORMAL;
        commandStarted = true;
    }
}

void AMW_Command_Composite_Forced_Nav::updateStatus() {
    if (completed || failed)
        return;

    if (subCommands.empty()) {
        completed = true;
        return;
    }

    if (AC_Facade::getFacade()->destinationReached(destination, AMW_COMMAND_COMPOSITE_NAV_FORCED_DESTINATION_RADIUS)) {
            completed = true;
            currentState = COMPLETED;
            clearReservedCorridors();
            AMW_Corridor_Manager::getInstance()->markCorridorConflictResolved(AMW_Planner::getModuleIdentifier());
#ifdef AMW_COMMAND_DEBUG
            AC_CommunicationFacade::sendFormattedDebug(PSTR("Starting forced nav to <%.2f,%.2f>"), destination.x / 100, destination.y / 100);
#endif
            return;
    }



}
