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

#ifndef TESTENV
typedef char prog_char_t;

#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); \
                                  (prog_char_t *)&__c[0];
#endif

CommunicationFacadeStub* CommunicationFacadeStub::facade = 0;

void AC_CommunicationFacade::broadcastReservationRequest(uint8_t reservationId,
        AMW_List<AMW_Corridor*>* corridors) {
    CommunicationFacadeStub::getFacade()->broadcastReservationRequest(reservationId, corridors);
}
void AC_CommunicationFacade::broadcastCorridors(
        AMW_List<AMW_Corridor*>* corridors) {
    CommunicationFacadeStub::getFacade()->broadcastCorridors(corridors);
}

