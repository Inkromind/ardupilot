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

class AC_CommunicationFacade {
public:
    static AC_CommunicationFacade* getFacade(void) {
        if (!facade)
            facade = new AC_CommunicationFacade();
        return facade;
    }

    virtual void broadcastReservationRequest(uint8_t reservationId, AMW_List<AMW_Corridor*>* corridors);

private:
    static AC_CommunicationFacade* facade;
protected:
    AC_CommunicationFacade() {}
    virtual ~AC_CommunicationFacade() {}
};

#endif /* AC_COMMUNICATIONFACADE_H_ */
