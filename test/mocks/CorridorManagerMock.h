/*
 * CorridorManagerMock.h
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_CORRIDORMANAGERMOCK_H_
#define MOCKS_CORRIDORMANAGERMOCK_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <list>
#include <AMW_Corridors.h>

class CorridorManagerMock : public AMW_Corridor_Manager {
public:
    CorridorManagerMock() {
    }

    ~CorridorManagerMock() {
        while (!markCorridorsReservedLists.empty()) {
            std::list<AMW_Corridor*>* corridors = markCorridorsReservedLists.front();
            markCorridorsReservedLists.pop_front();
            delete corridors;
        }
        while (!reserveCorridorsLists.empty()) {
            std::list<AMW_Corridor*>* corridors = reserveCorridorsLists.front();
            reserveCorridorsLists.pop_front();
            delete corridors;
        }
        while (!freeCorridorsLists.empty()) {
            std::list<AMW_Corridor*>* corridors = freeCorridorsLists.front();
            freeCorridorsLists.pop_front();
            while (!corridors->empty()) {
                AMW_Corridor* corridor = corridors->front();
                corridors->pop_front();
                delete corridor;
            }
            delete corridors;
        }
    }

    std::list<std::list<AMW_Corridor*>*> markCorridorsReservedLists;
    std::list<std::list<AMW_Corridor*>*> freeCorridorsLists;
    std::list<std::list<AMW_Corridor*>*> reserveCorridorsLists;

    static void setInstance(CorridorManagerMock* instance) {
        AMW_Corridor_Manager::module = instance;
    }
    static void deleteInstance() {
        AMW_Corridor_Manager::module = 0;
    }
    bool markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors);

    void freeCorridors(AMW_List<AMW_Corridor*>* corridors);

    bool reserveCorridors(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors, uint8_t maxFailures);

    void markCorridorConflictResolved(const AMW_Module_Identifier* module);

    bool hasCorridorConflict(const AMW_Module_Identifier* module) const;

    bool corridorsAreReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) const;

    bool isReservingCorridors(const AMW_Module_Identifier* module) const;

    bool reservationHasFailed(const AMW_Module_Identifier* module) const;

    float getReservedAltitude(const AMW_Module_Identifier* module) const;
};

#endif /* MOCKS_CORRIDORMANAGERMOCK_H_ */
