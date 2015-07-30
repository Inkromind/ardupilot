/*
 * AC_CommunicationFacade.cpp
 *
 *  Created on: 27-jul.-2015
 *      Author: Arne
 */

#include <AC_CommunicationFacade.h>
#include <AMW_List.h>
#include <AMW_Corridors.h>
#include <stdint.h>
#include "CommunicationFacadeStub.h"

CommunicationFacadeStub* CommunicationFacadeStub::facade = 0;

void AC_CommunicationFacade::broadcastReservationRequest(uint8_t reservationId,
        AMW_List<AMW_Corridor*>* corridors) {
    CommunicationFacadeStub::getFacade()->broadcastReservationRequest(reservationId, corridors);
}
void AC_CommunicationFacade::broadcastCorridors(
        AMW_List<AMW_Corridor*>* corridors) {
    CommunicationFacadeStub::getFacade()->broadcastCorridors(corridors);
}

