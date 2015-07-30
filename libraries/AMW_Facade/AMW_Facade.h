/*
 * AMW_Facade.h
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_FACADE_H_
#define AMW_FACADE_H_

#include "../AP_Math/vector2.h"
#include <stdint.h>
#include <AMW_Corridors.h>
#include <AMW_List.h>
#include <AMW_Modules.h>

/**
 * Facade to the Autonomous MiddleWare
 * This serves as access point to the long term planner
 */
class AMW_Facade {

public:
    /**
     * Initialize the planner
     */
    static void initPlanner();

    /**
     * Resume the mission
     */
    static void resumeMission();

    /**
     * Pause the mission
     */
    static void pauseMission();

    /**
     * Return home
     */
    static void returnHome();

    /**
     * Add a packagerequest to the schedule at the cheapest position
     * or just get an estimate
     *
     * @param id Package ID
     * @param pickupLocation The location to pick up the package
     * @param deliveryLocation The location to deliver the package
     * @param estimate Set to try to only get an estimate for adding the package
     * @return The estimate
     */
    static float addPackage(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation, bool estimate);

    /**
     * Check a reservation request for any conflicts with our own corridors
     *
     * @param corridors The corridors being reserved
     * @return The first conflict found or 0 if no conflicts exist
     */
    static AMW_Corridor_Conflict* checkReservationRequest(const AMW_List<AMW_Corridor*>* corridors);

    /**
     * A conflict was received for a reservation
     *
     * @param reservatioId The id of the reservation containing the conflict
     * @param conflict The conflict
     */
    static void reservationConflictReceived(uint8_t reservationId, const AMW_Corridor_Conflict* conflict);

    /**
     * A broadcast of reserved corridors was received
     */
    static void receivedCorridorBroadcast(const AMW_List<AMW_Corridor*>* corridors);

    /**
     * Mark the given corridors as reserved by the given module.
     * This bypasses the reservation process.
     * If another module with an equal or higher priority already has corridors
     * reserved or is currently trying to reserve corridors, the given module
     * will not be able to reserve corridors.
     *
     * @return True if the module was able to mark the corridors as reserved. False otherwise
     */
    static bool markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors);

    /**
     * Free the given corridors.
     * All corridors will be unmarked as reserved, they will be removed
     * from any running reservation attempts and the objects will be
     * deleted.
     */
    static void freeCorridors(AMW_List<AMW_Corridor*>* corridors);

    /**
     * Check if the battery has been marked as empty
     */
    static bool isBatteryEmpty(void);

    /**
     * Mark the battery as empty
     */
    static void markBatteryEmpty(void);

    static void run100Hz();

    static void run50Hz();

    static void run10Hz();

    static void run1Hz();

private:
    AMW_Facade();
    ~AMW_Facade();
};

#endif /* AMW_FACADE_H_ */
