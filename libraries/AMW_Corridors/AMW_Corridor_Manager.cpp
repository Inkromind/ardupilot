/*
 * AMW_Corridor_Manager.cpp
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Manager.h"
#include <stdint.h>
#include <stdlib.h>
#include "AMW_Position_Corridor.h"
#include <AMW_Modules.h>
#include <AC_CommunicationFacade.h>
#include <AC_Facade.h>

AMW_Corridor_Manager* AMW_Corridor_Manager::instance = 0;
uint8_t AMW_Corridor::nextId = 0;

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
    minAltitude = 0;

    resetLogging();
}

bool AMW_Corridor_Manager::reserveCorridors(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors, uint8_t newMaxFailures, float newMinAltitude) {
    if (!module || !corridors || corridors->empty() || !canReserveCorridors(module))
        return false;
    setNewModule(module);

    if (currentState != IDLE)
        this->totalResFailures++;
    preliminaryCorridors.clear();
    currentState = IDLE;

    if (newMinAltitude > AMW_CORRIDOR_MAX_ALTITUDE)
        return false;
    else if (newMinAltitude < AMW_CORRIDOR_MIN_ALTITUDE)
        minAltitude = AMW_CORRIDOR_MIN_ALTITUDE;
    else
        minAltitude = newMinAltitude;

    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setAltitude(minAltitude);
        preliminaryCorridors.push_back(corridor);
    }
    delete iterator;

    failures = 0;
    failed = false;
    preliminaryAltitude = minAltitude;
    this->maxFailures = newMaxFailures;
    this->totalRetries++;
    startReservationRound();

    return true;
}

AMW_Corridor_Manager* AMW_Corridor_Manager::getInstance(void) {
    if (!AMW_Corridor_Manager::instance) {
        AMW_Corridor_Manager::instance = new AMW_Corridor_Manager();
    }
    return AMW_Corridor_Manager::instance;
}

bool AMW_Corridor_Manager::canReserveCorridors(const AMW_Module_Identifier* module) const {
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

void AMW_Corridor_Manager::setNewModule(const AMW_Module_Identifier* module) {
    if (!module)
        return;
    if (reservedModule != module) {
        reservedCorridors.clear();
        reservedModule = module;
        corridorConflict = false;
        reservedAltitude = 0;
        if (currentState != IDLE)
            this->totalResFailures++;
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
    this->totalRounds++;
    increaseReservationId();
    AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        corridor->setAltitude(preliminaryAltitude);
    }
    delete iterator;

#ifdef AMW_CORRIDOR_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Attempting to reserve corridors at altitude %.2f"), preliminaryAltitude / 100);
#endif

    startWait(AMW_CORRIDOR_RESERVATION_TIMEOUT, AMW_CORRIDOR_RESERVATION_TIMEOUT_MAX_DELTA);
    AC_CommunicationFacade::broadcastReservationRequest(
            currentReservationId, &preliminaryCorridors);
    currentState = REQUEST_SEND;
}

void AMW_Corridor_Manager::startWait(float timeout, uint8_t maxDelta) {
    waitStart = AC_Facade::getFacade()->getTimeMillis();
    waitTimeout = timeout + rand() % maxDelta;
}

void AMW_Corridor_Manager::checkTimeout(void) {
    if (preliminaryCorridors.empty())
        return;
    if (currentState == IDLE)
        return;
    if (AC_Facade::getFacade()->getTimeMillis() - waitStart > waitTimeout * 1000) {
        if (currentState == REQUEST_SEND) {
            currentState = IDLE;
            reservedAltitude = preliminaryAltitude;
            AMW_List<AMW_Corridor*>::Iterator* iterator = preliminaryCorridors.iterator();
            while (iterator->hasNext()) {
                AMW_Corridor* corridor = iterator->next();
                reservedCorridors.push_back(corridor);
            }
            delete iterator;
            preliminaryCorridors.clear();

            this->totalCompletedRes++;
            this->sumLevels += reservedAltitude/100;

#ifdef AMW_CORRIDOR_DEBUG
            AC_CommunicationFacade::sendFormattedDebug(PSTR("Corridors reserved at altitude %.2f"), reservedAltitude / 100);
#endif
        }
        else if (currentState == WAITING_FOR_RETRY) {
            preliminaryAltitude = minAltitude;
            this->totalRetries++;
            startReservationRound();
        }
        else if (currentState == WAITING_FOR_NEXT_ROUND) {
            startReservationRound();
        }
    }
}

bool AMW_Corridor_Manager::corridorsAreReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) const {
    if (reservedModule != module || !module)
        return false;
    if (!corridors || corridors->empty())
        return false;
    if (corridors->size() > reservedCorridors.size())
        return false;

    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    bool allReserved = true;
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        AMW_List<AMW_Corridor*>::Iterator* iterator2 = reservedCorridors.iterator();
        bool reserved = false;
        while (iterator2->hasNext()) {
            if (corridor == iterator2->next()) {
                reserved = true;
                break;
            }
        }
        delete iterator2;
        if (!reserved) {
            allReserved = false;
            break;
        }
    }
    delete iterator;

    return allReserved;
}

bool AMW_Corridor_Manager::isReservingCorridors(const AMW_Module_Identifier* module) const {
    return (!preliminaryCorridors.empty() && module == reservedModule && module);
}

void AMW_Corridor_Manager::markCorridorConflictResolved(const AMW_Module_Identifier* module) {
    if (module == reservedModule)
        corridorConflict = false;
}

bool AMW_Corridor_Manager::markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) {
    if (!module || !canReserveCorridors(module) || !corridors || corridors->empty())
        return false;
    setNewModule(module);
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    float altitude = 0;
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        if (altitude == 0 && corridor->getAltitude() != 0) {
            altitude = corridor->getAltitude();
        }
        reservedCorridors.push_back(corridor);
    }
    delete iterator;
    if (altitude != 0)
        reservedAltitude = altitude;
#ifdef AMW_CORRIDOR_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Marking corridors as reserved at altitude %.2f"), reservedAltitude / 100);
#endif
    return true;
}

void AMW_Corridor_Manager::freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
    if (!corridors || corridors->empty())
        return;
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();

        uint32_t id = reservedCorridors.size();
        while (id > 0) {
            id--;
            AMW_Corridor* reservedCorridor = reservedCorridors.get(id);
            if (reservedCorridor == corridor)
                reservedCorridors.erase(id);
        }

        id = preliminaryCorridors.size();
        while (id > 0) {
            id--;
            AMW_Corridor* preliminaryCorridor = preliminaryCorridors.get(id);
            if (preliminaryCorridor == corridor)
                preliminaryCorridors.erase(id);
        }

        delete corridor;
    }
    delete iterator;

    if (reservedCorridors.empty()) {
        reservedAltitude = 0;
        corridorConflict = false;
    }

    if (preliminaryCorridors.empty()) {
        if (currentState != IDLE)
            this->totalResFailures++;
        currentState = IDLE;
    }

    if (reservedCorridors.empty() && preliminaryCorridors.empty()) {
        reservedModule = 0;
    }
}

AMW_Corridor_Conflict* AMW_Corridor_Manager::checkReservationRequest(const AMW_List<AMW_Corridor*>* corridors) const {
    if (!corridors || corridors->empty())
        return 0;
    AMW_Corridor_Conflict* conflict = 0;
    if (reservedCorridors.empty()) {
        AMW_Position_Corridor positionCorridor(AC_Facade::getFacade()->getRealPosition());
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

void AMW_Corridor_Manager::reservationConflictReceived(uint8_t reservationId, const AMW_Corridor_Conflict* conflict) {
    if (!conflict)
        return;

    if (this->currentReservationId != reservationId)
        return;

    if (currentState != REQUEST_SEND)
        return;

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
        this->totalResFailures++;
#ifdef AMW_CORRIDOR_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Reservation failed."));
#endif
        preliminaryCorridors.clear();
        failed = true;
    }
    else {
#ifdef AMW_CORRIDOR_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Reservation failed. Waiting before retry."));
#endif
        startWait(AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY, AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY_MAX_DELTA);
        currentState = WAITING_FOR_RETRY;
    }
}

void AMW_Corridor_Manager::roundFailed(void) {
    preliminaryAltitude += AMW_CORRIDOR_ALTITUDE_HEIGHT;
    if (preliminaryAltitude > AMW_CORRIDOR_MAX_ALTITUDE) {
#ifdef AMW_CORRIDOR_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Reservation failed. Max altitude reached."));
#endif
        reservationFailed();
    }
    else {
#ifdef AMW_CORRIDOR_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Reservation at altitude failed. Waiting before trying next altitude."));
#endif
        startWait(AMW_CORRIDOR_RESERVATION_ROUND_DELAY, AMW_CORRIDOR_RESERVATION_ROUND_DELAY_MAX_DELTA);
        currentState = WAITING_FOR_NEXT_ROUND;
    }
}

void AMW_Corridor_Manager::broadcastReservedCorridors(void) {
    if (reservedCorridors.empty()) {
        AMW_List<AMW_Corridor*> corridors;
        AMW_Position_Corridor positionCorridor(AC_Facade::getFacade()->getRealPosition(), 0);
        corridors.push_back(&positionCorridor);
        AC_CommunicationFacade::broadcastCorridors(&corridors);
    }
    else {
        AC_CommunicationFacade::broadcastCorridors(&reservedCorridors);
    }
}

void AMW_Corridor_Manager::receivedCorridorBroadcast(const AMW_List<AMW_Corridor*>* corridors) {
    if (!corridors || corridors->empty())
        return;

    AMW_Corridor_Conflict* conflict = 0;
    AMW_List<AMW_Corridor*>::Iterator* iterator = reservedCorridors.iterator();
    while (iterator->hasNext()) {
        conflict = iterator->next()->checkConflicts(corridors, false);
        if (conflict)
            break;
    }
    delete iterator;
    if (conflict) {
#ifdef AMW_CORRIDOR_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Corridor conflict detected."));
#endif
        corridorConflict = true;
        delete conflict;
    }
}

void AMW_Corridor_Manager::resetLogging(void) {
    totalRetries = 0;
    totalResFailures = 0;
    totalCompletedRes = 0;
    totalRounds = 0;
    sumLevels = 0;
}
