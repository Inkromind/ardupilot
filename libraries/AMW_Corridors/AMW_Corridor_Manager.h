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

#define AMW_CORRIDOR_MIN_ALTITUDE 500
#define AMW_CORRIDOR_ALTITUDE_HEIGHT 500
#define AMW_CORRIDOR_MAX_ALTITUDE 3000

#ifndef UINT8_MAX
#define AMW_MAX_RESERVATION_ID 0xff
#else
#define AMW_MAX_RESERVATION_ID UINT8_MAX
#endif

#define AMW_CORRIDOR_RESERVATION_TIMEOUT 5
#define AMW_CORRIDOR_RESERVATION_TIMEOUT_MAX_DELTA 5
#define AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY 30
#define AMW_CORRIDOR_RESERVATION_FAILURE_RETRY_DELAY_MAX_DELTA 5
#define AMW_CORRIDOR_RESERVATION_ROUND_DELAY 0
#define AMW_CORRIDOR_RESERVATION_ROUND_DELAY_MAX_DELTA 5

#define AMW_CORRIDOR_MAX_FAILURES 5

/**
 * Manages reservations of flight corridors
 */
class AMW_Corridor_Manager {
public:

    /**
     * Attempt to reserve the given set of corridors.
     * If another module with an equal or higher priority already has corridors
     * reserved or is currently trying to reserve corridors, the given module
     * will not be able to reserve corridors.
     * If the given module is already trying to reserve corridors, the attempt
     * will be cancelled in favor of the new one.
     * Corridors reserved by the given module will stay reserved.
     *
     * @param module The module attempting to reserve the corridors
     * @param corridors The list of corridors to reserve
     * @param maxFailures The maximum number of tries before giving up
     * @return True if the reservation process was started, false otherwise
     */
    virtual bool reserveCorridors(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors, uint8_t maxFailures = AMW_CORRIDOR_MAX_FAILURES);

    /**
     * Check if the given set of corridors is reserved to the given module
     */
    virtual bool corridorsAreReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) const;

    /**
     * Check if the last reservation has failed and was
     * performed by the given module
     */
    virtual bool reservationHasFailed(const AMW_Module_Identifier* module) const {
        if (reservedModule != module || !module)
            return true;
        return failed;
    }

    /**
     * Check if the given module is attempting to reserve corridors
     */
    virtual bool isReservingCorridors(const AMW_Module_Identifier* module) const;

    /**
     * Check if there is conflict for corridors reserved by the given module
     */
    virtual bool hasCorridorConflict(const AMW_Module_Identifier* module) const {
        if (reservedModule != module || !module)
            return false;
        return corridorConflict;
    }

    /**
     * Mark any conflicts for the given corridor as resolved
     */
    virtual void markCorridorConflictResolved(const AMW_Module_Identifier* module);

    /**
     * Mark the given corridors as reserved by the given module.
     * This bypasses the reservation process.
     * If another module with an equal or higher priority already has corridors
     * reserved or is currently trying to reserve corridors, the given module
     * will not be able to reserve corridors.
     *
     * @return True if the module was able to mark the corridors as reserved. False otherwise
     */
    virtual bool markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors);

    /**
     * Get the altitude of the last succesfull reservation for module.
     */
    virtual float getReservedAltitude(const AMW_Module_Identifier* module) const {
        if (reservedModule != module || !module)
            return 0;
        return reservedAltitude;
    }

    /**
     * Get the instance of the manager
     */
    static AMW_Corridor_Manager* getInstance(void);

    /**
     * Free the given corridors.
     * All corridors will be unmarked as reserved, they will be removed
     * from any running reservation attempts and the objects will be
     * deleted.
     */
    virtual void freeCorridors(AMW_List<AMW_Corridor*>* corridors);

    /**
     * Check if any part of the reservation process has timed out.
     */
    virtual void checkTimeout(void);

    /**
     * Check a received reservation request for any conflicts
     */
    virtual AMW_Corridor_Conflict* checkReservationRequest(const AMW_List<AMW_Corridor*>* corridors) const;

    /**
     * A conflict for a reservation request was received
     */
    virtual void reservationConflictReceived(uint8_t reservationId, const AMW_Corridor_Conflict* conflict);

    /**
     * Broadcast the currently reserved corridors
     */
    virtual void broadcastReservedCorridors(void);

    /**
     * A broadcast of reserved corridors was received
     */
    virtual void receivedCorridorBroadcast(const AMW_List<AMW_Corridor*>* corridors);

    enum State { IDLE, REQUEST_SEND, WAITING_FOR_RETRY, WAITING_FOR_NEXT_ROUND };

    /**
     * Check if the given module can reserve corridors
     */
    bool canReserveCorridors(const AMW_Module_Identifier* module) const;
protected:
    virtual ~AMW_Corridor_Manager() { }

    static AMW_Corridor_Manager* module;
    AMW_Corridor_Manager();

    AMW_List<AMW_Corridor*> reservedCorridors;
    AMW_List<AMW_Corridor*> preliminaryCorridors;

    uint8_t currentReservationId;
    uint8_t failures;
    float reservedAltitude;
    float preliminaryAltitude;
    uint32_t waitStart;
    float waitTimeout;
    bool failed;
    State currentState;
    uint8_t maxFailures;

    bool corridorConflict;
    const AMW_Module_Identifier* reservedModule;

private:

    /**
     * Set the given module as the currently reserving module
     */
    void setNewModule(const AMW_Module_Identifier* module);

    /**
     * Start a timer
     */
    void startWait(float timeout, uint8_t maxDelta);

    /**
     * The reservation has failed for all altitudes
     */
    void reservationFailed(void);

    /**
     * The reservation has failed for the current altitude
     */
    void roundFailed(void);

    /**
     * Attempt to reserve the preliminary corridors for the next altitude
     */
    void startReservationRound();

    /**
     * Increase the reservation id
     */
    void increaseReservationId();

};

#endif /* AMW_CORRIDOR_MANAGER_H_ */
