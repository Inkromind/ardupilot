/*
 * AC_CommunicationFacade.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AC_COMMUNICATIONFACADE_H_
#define AC_COMMUNICATIONFACADE_H_

#include <stdint.h>
#include <AMW_List.h>
#include <AMW_Corridors.h>
#ifndef TESTENV
    #include <AP_Progmem.h>
#endif

class AC_CommunicationFacade {
public:
    /**
     * Broadcast a reservation request
     */
    static void broadcastReservationRequest(uint8_t reservationId, AMW_List<AMW_Corridor*>* corridors);

    /**
     * Broadcast currently assigned corridors
     */
    static void broadcastCorridors(AMW_List<AMW_Corridor*>* corridors);

    /**
     * Mark a package as completed
     */
    static void completedPackage(uint8_t packageId);

    /**
     * Mark a package as completed
     */
    static void failedPackage(uint8_t packageId);

    static void sendLogging(float xPos, float yPos, float curAlt, float assAlt, float deviation);


#ifndef TESTENV
    static void sendDebug(const prog_char_t *str);
    static void sendFormattedDebug(const prog_char_t *str, ...);
#endif


private:
    AC_CommunicationFacade() {}
protected:
    virtual ~AC_CommunicationFacade() {}
};

#endif /* AC_COMMUNICATIONFACADE_H_ */
