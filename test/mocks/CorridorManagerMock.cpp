/*
 * CorridorManagerMock.cpp
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#include "CorridorManagerMock.h"

bool CorridorManagerMock::markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) {
    if (corridors) {
        std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            corridorList->push_back(iterator->next());
        }
        delete iterator;
        markCorridorsReservedLists.push_back(corridorList);
    }
    return mock("CorManager").actualCall("markCorridorsReserved").onObject(this).returnIntValueOrDefault(1);
}

bool CorridorManagerMock::reserveCorridors(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors, uint8_t maxFailures) {
    if (corridors) {
        std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            corridorList->push_back(iterator->next());
        }
        delete iterator;
        reserveCorridorsLists.push_back(corridorList);
    }
    return mock("CorManager").actualCall("reserveCorridors").onObject(this).withParameter("maxFailures", maxFailures).returnIntValueOrDefault(1);
}

void CorridorManagerMock::freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
    if (corridors) {
        std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            corridorList->push_back(iterator->next());
        }
        delete iterator;
        freeCorridorsLists.push_back(corridorList);
    }
    mock("CorManager").actualCall("freeCorridors").onObject(this);
}

void CorridorManagerMock::markCorridorConflictResolved(const AMW_Module_Identifier* module) {
    mock("CorManager").actualCall("markCorridorConflictResolved").onObject(this);
}

bool CorridorManagerMock::hasCorridorConflict(const AMW_Module_Identifier* module) const {
    return mock("CorManager").actualCall("hasCorridorConflict").onObject(const_cast<CorridorManagerMock*>(this)).returnIntValueOrDefault(1);
}

bool CorridorManagerMock::corridorsAreReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) const {
    return mock("CorManager").actualCall("corridorsAreReserved").onObject(const_cast<CorridorManagerMock*>(this)).returnIntValueOrDefault(1);
}

bool CorridorManagerMock::isReservingCorridors(const AMW_Module_Identifier* module) const {
    return mock("CorManager").actualCall("isReservingCorridors").onObject(const_cast<CorridorManagerMock*>(this)).returnIntValueOrDefault(1);
}
