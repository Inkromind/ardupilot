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
#include "AMW_Corridors.h"
#include "AMW_Corridor_Reservation.h"
#include <stdint.h>
#include "AMW_Corridor_Conflict.h"

#define AMW_CORRIDOR_MIN_ALTITUDE 5

#ifndef UINT8_MAX
#define AMW_MAX_RESERVATION_ID 0xff
#else
#define AMW_MAX_RESERVATION_ID UINT8_MAX
#endif

#define AMW_CORRIDOR_RESERVATION_TIMEOUT 10

class AMW_Corridor_Manager {
public:
    virtual ~AMW_Corridor_Manager() { }

    bool reserveCorridors(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors);

    bool corridorsAreReserved(AMW_List<AMW_Corridor*>* corridors);
    bool reservationFailed(AMW_Module_Identifier* module) {
        return failed;
    }
    bool isReservingCorridors(AMW_Module_Identifier* module);
    bool hasCorridorConflict(AMW_Module_Identifier* module) {
        return corridorConflict;
    }
    void markCorridorConflictResolved(AMW_Module_Identifier* module);
    bool markCorridorsReserved(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors);
    bool getReservedAltitude(AMW_Module_Identifier* module) {
        return reservedAltitude;
    }

    static AMW_Corridor_Manager* getInstance(void);

    void freeCorridors(AMW_List<AMW_Corridor*>* corridors);

    void checkTimeout(void);

    AMW_Corridor_Conflict* checkReservationRequest(AMW_List<AMW_Corridor*>* corridors);

    void reservationConflictReceived(uint8_t reservationId, AMW_Corridor_Conflict* conflict);

    void broadcastReservedCorridors(void);

    void receivedCorridorBroadcast(AMW_List<AMW_Corridor*>* corridors);



private:
    static AMW_Corridor_Manager* module;
    AMW_Corridor_Manager();

    AMW_Module_Identifier* reservedModule;

    bool canReserveCorridors(AMW_Module_Identifier* module);

    void setNewModule(AMW_Module_Identifier* module);

    void startReservation(void);

    AMW_List<AMW_Corridor*> reservedCorridors;
    AMW_List<AMW_Corridor*> preliminaryCorridors;

    uint16_t currentReservationId;
    uint8_t failures;
    float reservedAltitude;
    float preliminaryAltitude;
    uint32_t reservationStart;
    float reservationTimeout;
    bool failed;

    bool corridorConflict;

};

#endif /* AMW_CORRIDOR_MANAGER_H_ */
