/*
 * AMW_Corridor_Manager.h
 *
 *  Created on: 11-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_MANAGER_H_
#define AMW_CORRIDOR_MANAGER_H_

#include <AMW_Modules.h>
#include <AMW_List.h>
#include "AMW_Corridor.h"
#include <stdint.h>

class AMW_Corridor_Conflict;

#define AMW_CORRIDOR_MIN_ALTITUDE 5000
#define AMW_CORRIDOR_ALTITUDE_HEIGHT 5000
#define AMW_CORRIDOR_MAX_ALTITUDE 30000

#ifndef UINT8_MAX
#define AMW_MAX_RESERVATION_ID 0xff
#else
#define AMW_MAX_RESERVATION_ID UINT8_MAX
#endif

#define AMW_CORRIDOR_RESERVATION_TIMEOUT 10
#define AMW_CORRIDOR_RESERVATION_TIMEOUT_MAX_DELTA 5
#define AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY 30
#define AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY_MAX_DELTA 5
#define AMW_CORRIDOR_RESERVATION_ROUND_DELAY 0
#define AMW_CORRIDOR_RESERVATION_ROUND_DELAY_MAX_DELTA 5

#define AMW_CORRIDOR_MAX_FAILURES 5

class AMW_Corridor_Manager {
public:
    virtual ~AMW_Corridor_Manager() { }

    virtual bool reserveCorridors(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors, uint8_t maxFailures = AMW_CORRIDOR_MAX_FAILURES);

    virtual bool corridorsAreReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) const;
    virtual bool reservationHasFailed(const AMW_Module_Identifier* module) const {
        return failed;
    }
    virtual bool isReservingCorridors(const AMW_Module_Identifier* module) const;
    virtual bool hasCorridorConflict(const AMW_Module_Identifier* module) const {
        return corridorConflict;
    }
    virtual void markCorridorConflictResolved(const AMW_Module_Identifier* module);
    virtual bool markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors);
    virtual bool getReservedAltitude(const AMW_Module_Identifier* module) const {
        return reservedAltitude;
    }

    static AMW_Corridor_Manager* getInstance(void);

    virtual void freeCorridors(AMW_List<AMW_Corridor*>* corridors);

    virtual void checkTimeout(void);

    virtual AMW_Corridor_Conflict* checkReservationRequest(const AMW_List<AMW_Corridor*>* corridors) const;

    virtual void reservationConflictReceived(uint8_t reservationId, const AMW_Corridor_Conflict* conflict);

    virtual void broadcastReservedCorridors(void);

    virtual void receivedCorridorBroadcast(const AMW_List<AMW_Corridor*>* corridors);

protected:
    static AMW_Corridor_Manager* module;
    AMW_Corridor_Manager();

private:
    enum State { IDLE, REQUEST_SEND, WAITING_FOR_RETRY, WAITING_FOR_NEXT_ROUND };

    const AMW_Module_Identifier* reservedModule;

    bool canReserveCorridors(const AMW_Module_Identifier* module) const;

    void setNewModule(const AMW_Module_Identifier* module);

    void startWait(float timeout, uint8_t maxDelta);

    void reservationFailed(void);
    void roundFailed(void);
    void startReservationRound();
    void increaseReservationId();

    AMW_List<AMW_Corridor*> reservedCorridors;
    AMW_List<AMW_Corridor*> preliminaryCorridors;

    uint16_t currentReservationId;
    uint8_t failures;
    float reservedAltitude;
    float preliminaryAltitude;
    uint32_t waitStart;
    float waitTimeout;
    bool failed;
    State currentState;
    bool maxFailures;

    bool corridorConflict;

};

#endif /* AMW_CORRIDOR_MANAGER_H_ */
