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
    std::list<AMW_Module_Identifier*> markCorridorsReservedModules;

    static void setInstance(CorridorManagerMock* instance) {
        AMW_Corridor_Manager::module = instance;
    }
    static void deleteInstance() {
        AMW_Corridor_Manager::module = 0;
    }
    bool markCorridorsReserved(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors) {
        std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            corridorList->push_back(iterator->next());
        }
        delete iterator;
        markCorridorsReservedLists.push_back(corridorList);
        markCorridorsReservedModules.push_back(module);
        return mock().actualCall("markCorridorsReserved").onObject(this).returnIntValue();
    }

    void freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
        std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            corridorList->push_back(iterator->next());
        }
        delete iterator;
        freeCorridorsLists.push_back(corridorList);
        mock().actualCall("freeCorridors").onObject(this);
    }


};

#endif /* MOCKS_CORRIDORMANAGERMOCK_H_ */
