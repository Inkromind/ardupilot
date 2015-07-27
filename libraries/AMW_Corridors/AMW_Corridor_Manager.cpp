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
#include "AMW_Position_Corridor.h"

AMW_Corridor_Manager* AMW_Corridor_Manager::module = 0;

AMW_Corridor_Manager::AMW_Corridor_Manager() {
    reservedModule = 0;
    currentReservationId = 0;
    failures = 0;
    reservedAltitude = 0;
    preliminaryAltitude = 0;
    waitStart = 0;
    waitTimeout = 0;
    failed = false;
    corridorConflict = false;
    currentState = IDLE;
    maxFailures = 0;
}

bool AMW_Corridor_Manager::reserveCorridors(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors, uint8_t maxFailures) {
    if (!module || !corridors || corridors->empty() || !canReserveCorridors(module))
        return false;
    setNewModule(module);

    preliminaryCorridors.clear();
    currentState = IDLE;

    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setAltitude(AMW_CORRIDOR_MIN_ALTITUDE);
        preliminaryCorridors.push_back(corridor);
    }
    delete iterator;

    failures = 0;
    failed = false;
    preliminaryAltitude = AMW_CORRIDOR_MIN_ALTITUDE;
    this->maxFailures = maxFailures;
    startReservationRound();

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
    if (!module)
        return;
    if (reservedModule != module) {
        reservedCorridors.clear();
        reservedModule = module;
        corridorConflict = false;
        reservedAltitude = 0;
        preliminaryCorridors.clear();
        currentState = IDLE;
    }
}

void AMW_Corridor_Manager::increaseReservationId() {
    if (currentReservationId >= AMW_MAX_RESERVATION_ID)
        currentReservationId = 0;
    else
        currentReservationId++;
}

void AMW_Corridor_Manager::startReservationRound() {
    increaseReservationId();
    AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setAltitude(preliminaryAltitude);
    }
    delete iterator;

    startWait(AMW_CORRIDOR_RESERVATION_TIMEOUT, AMW_CORRIDOR_RESERVATION_TIMEOUT_MAX_DELTA);
    AC_CommunicationFacade::broadcastReservationRequest(
            currentReservationId, &preliminaryCorridors);
    currentState = REQUEST_SEND;
}

void AMW_Corridor_Manager::startWait(float timeout, uint8_t maxDelta) {
    waitStart = hal.scheduler->millis();
    waitTimeout = timeout + rand() % maxDelta;
}

void AMW_Corridor_Manager::checkTimeout(void) {
    if (preliminaryCorridors.empty())
        return;
    if (currentState == IDLE)
        return;
    if (hal.scheduler->millis() - waitStart > waitTimeout * 1000) {
        currentState = IDLE;
        if (currentState == REQUEST_SEND) {
            reservedAltitude = preliminaryAltitude;
            AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
            while (iterator->hasNext()) {
                AMW_Corridor* corridor = iterator->next();
                reservedCorridors.push_back(corridor);
            }
            delete iterator;
            preliminaryCorridors.clear();
        }
        else if (currentState == WAITING_FOR_RETRY) {
            preliminaryAltitude = AMW_CORRIDOR_MIN_ALTITUDE;
            startReservationRound();
        }
        else if (currentState == WAITING_FOR_NEXT_ROUND) {
            startReservationRound();
        }
    }
}

bool AMW_Corridor_Manager::corridorsAreReserved(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors) {
    if (reservedModule != module)
        return false;
    if (!corridors)
        return false;
    if (corridors->size() > reservedCorridors.size())
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
    if (!module || !canReserveCorridors(module))
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
    if (preliminaryCorridors.empty()) {
        currentState = IDLE;
    }
}

AMW_Corridor_Conflict* AMW_Corridor_Manager::checkReservationRequest(AMW_List<AMW_Corridor*>* corridors) {
    AMW_Corridor_Conflict* conflict = 0;
    if (reservedCorridors.empty()) {
        AMW_Position_Corridor positionCorridor = AMW_Position_Corridor<AC_Facade::getFacade()->getRealPosition()>;
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
        if (!conflict) {
            AMW_List<AMW_Corridor*>::Iterator* iterator2 = preliminaryCorridors.iterator();
            while (iterator2->hasNext()) {
                conflict = iterator2->next()->checkConflicts(corridors, true);
                if (conflict)
                    break;
            }
            delete iterator2;
        }
    }
    return conflict;
}

void AMW_Corridor_Manager::reservationConflictReceived(uint8_t reservationId, AMW_Corridor_Conflict* conflict) {
    if (this->currentReservationId != reservationId)
        return;

    if (currentState != REQUEST_SEND)
        return;

    bool validCorridor = false;
    AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
    while (iterator->hasNext()) {
        if (iterator->next()->getId() == conflict->getOwnId()) {
            validCorridor = true;
            break;
        }
    }
    delete iterator;
    if (!validCorridor)
        return;

    increaseReservationId();
    currentState = IDLE;
    if (conflict->getOwnType() == AMW_Corridor::VERTICAL) {
        reservationFailed();
    }
    else {
        if (conflict->getOtherType() == AMW_Corridor::VERTICAL) {
            preliminaryAltitude = conflict->getOtherAltitude();
        }
        roundFailed();
    }
}

void AMW_Corridor_Manager::reservationFailed(void) {
    failures++;
    if (failures >= maxFailures) {
        preliminaryCorridors.clear();
        failed = true;
    }
    else {
        startWait(AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY, AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY_MAX_DELTA);
        currentState = WAITING_FOR_RETRY;
    }
}

void AMW_Corridor_Manager::roundFailed(void) {
    preliminaryAltitude += AMW_CORRIDOR_ALTITUDE_HEIGHT;
    if (preliminaryAltitude > AMW_CORRIDOR_MAX_ALTITUDE) {
        reservationFailed();
    }
    else {
        startWait(AMW_CORRIDOR_RESERVATION_ROUND_DELAY, AMW_CORRIDOR_RESERVATION_ROUND_DELAY_MAX_DELTA);
        currentState = WAITING_FOR_NEXT_ROUND;
    }
}

void AMW_Corridor_Manager::broadcastReservedCorridors(void) {
    if (reservedCorridors.empty()) {
        AMW_List<AMW_Corridor*> corridors;
        AMW_Position_Corridor positionCorridor = AMW_Position_Corridor(AC_Facade::getFacade()->getRealPosition());
        corridors.push_back(&positionCorridor);
        AC_CommunicationFacade::broadcastCorridors(&corridors);
    }
    else {
        AC_CommunicationFacade::broadcastCorridors(&reservedCorridors);
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
