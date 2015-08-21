/*
 * AC_CommunicationFacade.h
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#ifndef STUBS_COMMUNICATIONFACADESTUB_H_
#define STUBS_COMMUNICATIONFACADESTUB_H_

#include <AMW_List.h>
#include "../../libraries/AMW_Corridors/AMW_Corridor.h"

class CommunicationFacadeStub {
public:
    /**
     * Return the instance of the facade
     */
    static CommunicationFacadeStub* getFacade(void) {
        if (!facade)
            facade = new CommunicationFacadeStub();
        return facade;
    }

    virtual ~CommunicationFacadeStub() {};

    virtual void broadcastReservationRequest(uint8_t reservationId,
            AMW_List<AMW_Corridor*>* corridors) {
    }
    virtual void broadcastCorridors(
            AMW_List<AMW_Corridor*>* corridors) {
    }
    virtual void completedPackage(uint8_t packageId) {
    }
    virtual void failedPackage(uint8_t packageId) {
    }

protected:
    static CommunicationFacadeStub* facade;
    CommunicationFacadeStub() {}
};

#endif /* STUBS_COMMUNICATIONFACADESTUB_H_ */
