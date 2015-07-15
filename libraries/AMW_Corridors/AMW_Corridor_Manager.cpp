/*
 * AMW_Corridor_Manager.cpp
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Manager.h"
#include <stdint.h>
#include <AP_HAL.h>
#include <stdlib.h>

AMW_Corridor_Manager* AMW_Corridor_Manager::module = 0;

AMW_Corridor_Manager::AMW_Corridor_Manager() {
    reservedModule = 0;
    currentReservationId = 0;
    failures = 0;
    reservedAltitude = 0;
    preliminaryAltitude = 0;
    reservationStart = 0;
    reservationTimeout = 0;
    failed = false;
    corridorConflict = false;
}

bool AMW_Corridor_Manager::reserveCorridors(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors) {
    if (!corridors || corridors->empty() || !canReserveCorridors(module))
        return false;
    setNewModule(module);

    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setAltitude(AMW_CORRIDOR_MIN_ALTITUDE);
        preliminaryCorridors.push_back(corridor);
    }
    delete iterator;

    startReservation();

    return true;
}

AMW_Corridor_Manager* AMW_Corridor_Manager::getInstance(void) {
    if (!AMW_Corridor_Manager::module) {
        AMW_Corridor_Manager::module = new AMW_Corridor_Manager();
    }
    return AMW_Corridor_Manager::module;
}

bool AMW_Corridor_Manager::canReserveCorridors(AMW_Module_Identifier* module) {
    if (!module)
        return false;
    if (reservedCorridors.empty() && preliminaryCorridors.empty())
        return true;
    if (!reservedModule)
        return true;
    if (reservedModule == module)
        return true;
    return (reservedModule->getCorridorPriority() < module->getCorridorPriority());
}

void AMW_Corridor_Manager::setNewModule(AMW_Module_Identifier* module) {
    if (!reservedModule) {
        reservedModule = module;
        corridorConflict = false;
        reservedAltitude = 0;
    }
    else if (reservedModule != module) {
        reservedCorridors.clear();
        corridorConflict = false;
        preliminaryCorridors.clear();
        reservedModule = module;
        reservedAltitude = 0;
    }
    else {
        preliminaryCorridors.clear();
    }
}

void AMW_Corridor_Manager::startReservation(void) {
    failures = 0;
    failed = false;
    preliminaryAltitude = AMW_CORRIDOR_MIN_ALTITUDE;
    if (currentReservationId-1 >= AMW_MAX_RESERVATION_ID)
        currentReservationId = 0;


    currentReservationId++;
    reservationStart = hal.scheduler->millis();
    reservationTimeout = AMW_CORRIDOR_RESERVATION_TIMEOUT + rand() % 5;
    AC_CommunicationFacade::getFacade()->broadcastReservationRequest(currentReservationId, &preliminaryCorridors);
}

void AMW_Corridor_Manager::checkTimeout(void) {
    if (preliminaryCorridors.empty())
        return;
    if (hal.scheduler->millis() - reservationStart > reservationTimeout * 1000) {
        reservedAltitude = preliminaryAltitude;
        AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
        while (iterator->hasNext()) {
            AMW_Corridor* corridor = iterator->next();
            reservedCorridors.push_back(corridor);
        }
        delete iterator;
        preliminaryCorridors.clear();
    }
}

bool AMW_Corridor_Manager::corridorsAreReserved(AMW_List<AMW_Corridor*>* corridors) {
    if (!corridors)
        return false;
    AMW_List<AMW_Corridor*> corridorsCopy;
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        corridorsCopy.push_back(iterator->next());
    }
    delete iterator;
    AMW_List<AMW_Corridor*>::Iterator* iterator2 = reservedCorridors.iterator();
    while (iterator2->hasNext() && !corridorsCopy.empty()) {
        AMW_Corridor* corridor = iterator->next();
        uint32_t id = 0;
        while (id < corridorsCopy.size()) {
            AMW_Corridor* corridor2 = corridorsCopy.get(id);
            if (corridor2 == corridor) {
                corridorsCopy.erase(id);
                break;
            }
        }
    }
    delete iterator2;
    return (corridorsCopy.empty());
}

bool AMW_Corridor_Manager::isReservingCorridors(AMW_Module_Identifier* module) {
    return (!preliminaryCorridors.empty() && module == reservedModule);
}

void AMW_Corridor_Manager::markCorridorConflictResolved(AMW_Module_Identifier* module) {
    if (module == reservedModule)
        corridorConflict = false;
}

bool AMW_Corridor_Manager::markCorridorsReserved(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors) {
    if (!canReserveCorridors(module))
        return false;
    setNewModule(module);
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    float altitude = 0;
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        if (altitude == 0 && corridor->getAltitude() != 0) {
            altitude = corridor->getAltitude();
        }
        reservedAltitude = corridor->getAltitude();
        reservedCorridors.push_back(corridor);
    }
    delete iterator;
    if (altitude != 0)
        reservedAltitude = altitude;
    return true;
}

void AMW_Corridor_Manager::freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
    uint32_t id = reservedCorridors.size();
    while (id > 0) {
        id--;
        AMW_Corridor* corridor = reservedCorridors.get(id);
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            if (corridor == iterator->next()) {
                reservedCorridors.eraseAndDelete(id);
                break;
            }
        }
        delete iterator;
    }
    if (reservedCorridors.empty()) {
        reservedAltitude = 0;
        corridorConflict = false;
    }
    id = preliminaryCorridors.size();
    while (id > 0) {
        id--;
        AMW_Corridor* corridor = preliminaryCorridors.get(id);
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            if (corridor == iterator->next()) {
                preliminaryCorridors.eraseAndDelete(id);
                break;
            }
        }
        delete iterator;
    }
    if (reservedCorridors.empty() && preliminaryCorridors.empty()) {
        reservedModule = 0;
    }
}

AMW_Corridor_Conflict* AMW_Corridor_Manager::checkReservationRequest(AMW_List<AMW_Corridor*>* corridors) {
    AMW_Corridor_Conflict* conflict = 0;
    if (reservedCorridors.empty()) {
        PositionCorridor positionCorridor<AC_Facade::getFacade()->getRealPosition()>;
        conflict = positionCorridor.checkConflicts(corridors, true);
    }
    else {
        AMW_List<AMW_Corridor*>::Iterator* iterator = reservedCorridors.iterator();
        while (iterator->hasNext()) {
            conflict = iterator->next()->checkConflicts(corridors, true);
            if (conflict)
                break;
        }
        delete iterator;
    }
    return conflict;
}

void AMW_Corridor_Manager::reservationConflictReceived(uint8_t reservationId, AMW_Corridor_Conflict* conflict) {
    if (this->currentReservationId != reservationId)
        return;
    currentReservationId++;
    //TODO
}

void AMW_Corridor_Manager::broadcastReservedCorridors(void) {
    if (reservedCorridors.empty()) {
        AMW_List<AMW_Corridor*> corridors;
        PositionCorridor positionCorridor(AC_Facade::getFacade()->getRealPosition())
        corridors.push_back(&PositionCorridor);
        AC_CommunicationFacade::getFacade()->broadcastCorridors(&corridors);
    }
    else {
        AC_CommunicationFacade::getFacade()->broadcastCorridors(&reservedCorridors);
    }
}

void AMW_Corridor_Manager::receivedCorridorBroadcast(AMW_List<AMW_Corridor*>* corridors) {
    AMW_Corridor_Conflict* conflict = 0;
    AMW_List<AMW_Corridor*>::Iterator* iterator = reservedCorridors.iterator();
    while (iterator->hasNext()) {
        conflict = iterator->next()->checkConflicts(corridors, true);
        if (conflict)
            break;
    }
    delete iterator;
    if (conflict) {
        corridorConflict = true;
        delete conflict;
    }
}
