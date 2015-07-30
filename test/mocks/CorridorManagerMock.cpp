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
    return mock().actualCall("CMmarkCorridorsReserved").onObject(this).returnIntValue();
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
    mock().actualCall("CMfreeCorridors").onObject(this);
}
