/*
 * CommunicationFacadeMock.h
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_COMMUNICATIONFACADEMOCK_H_
#define MOCKS_COMMUNICATIONFACADEMOCK_H_

#include "../stubs/CommunicationFacadeStub.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <list>

class CommunicationFacadeMock : public CommunicationFacadeStub {
public:
    static void setFacade(CommunicationFacadeMock* facade) {
        CommunicationFacadeStub::facade = facade;
    }
    static void deleteFacade() {
        CommunicationFacadeStub::facade = 0;
    }

    void broadcastReservationRequest(uint8_t reservationId,
            AMW_List<AMW_Corridor*>* corridors) {
        if (corridors) {
            std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
            AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
            while (iterator->hasNext()) {
                corridorList->push_back(iterator->next());
            }
            delete iterator;
            broadcastReservationRequestLists.push_back(corridorList);
        }
        mock("ComFacade").actualCall("broadcastReservationRequest").onObject(this).withParameter("reservationId", reservationId);
    }
    void broadcastCorridors(
            AMW_List<AMW_Corridor*>* corridors) {
        if (corridors) {
            std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
            AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
            while (iterator->hasNext()) {
                corridorList->push_back(iterator->next());
            }
            delete iterator;
            broadcastCorridorsLists.push_back(corridorList);
        }
        mock("ComFacade").actualCall("broadcastCorridors").onObject(this);
    }

    ~CommunicationFacadeMock() {
        while (!broadcastReservationRequestLists.empty()) {
            std::list<AMW_Corridor*>* corridors = broadcastReservationRequestLists.front();
            broadcastReservationRequestLists.pop_front();
            delete corridors;
        }
        while (!broadcastCorridorsLists.empty()) {
            std::list<AMW_Corridor*>* corridors = broadcastCorridorsLists.front();
            broadcastCorridorsLists.pop_front();
            delete corridors;
        }
    }

    std::list<std::list<AMW_Corridor*>*> broadcastReservationRequestLists;
    std::list<std::list<AMW_Corridor*>*> broadcastCorridorsLists;

};



#endif /* MOCKS_COMMUNICATIONFACADEMOCK_H_ */
