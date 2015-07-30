/*
 * CorridorMock.h
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#ifndef STUBS_CORRIDORMOCK_H_
#define STUBS_CORRIDORMOCK_H_

#include "../../libraries/AMW_Corridors/AMW_Corridor.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <list>

class DummyCorridor: public AMW_Corridor {

public:
    DummyCorridor() : AMW_Corridor() {}
    DummyCorridor(uint8_t id) : AMW_Corridor(id) {}


    virtual AMW_Corridor::Type getType(void) const {
        return (AMW_Corridor::Type) mock("Corridor").actualCall("getType").onObject(const_cast<DummyCorridor*>(this)).returnIntValueOrDefault(1);
    }

    virtual void setReverseDirection(bool newValue) {
        mock("Corridor").actualCall("setReverseDirection").onObject(this).withParameter("newValue", newValue);
    }

    virtual void setInCorridor(bool newValue) {
        mock("Corridor").actualCall("setInCorridor").onObject(this).withParameter("newValue", newValue);
    }

    virtual void setCompleted(bool newValue) {
        mock("Corridor").actualCall("setCompleted").onObject(this).withParameter("newValue", newValue);
    }

    virtual Vector3f getStartPoint(bool checkFullCorridor = true) const {
        return *((Vector3f*) mock("Corridor").actualCall("getStartPoint").onObject(const_cast<DummyCorridor*>(this)).withParameter("checkFullCorridor", checkFullCorridor).returnPointerValue());
    }
    virtual Vector3f getEndPoint() const {
        return *((Vector3f*) mock("Corridor").actualCall("getEndPoint").onObject(const_cast<DummyCorridor*>(this)).returnPointerValue());
    }

    virtual bool checkForConflicts(bool checkFullCorridor = true) const {
        return mock("Corridor").actualCall("checkForConflicts").onObject(const_cast<DummyCorridor*>(this)).withParameter("checkFullCorridor", checkFullCorridor).returnIntValueOrDefault(1);
    };
};

class CorridorMock : public DummyCorridor {
public:
    CorridorMock() {
        id = 0;
        altitude = 0;
    }

    virtual void setAltitude(float newAltitude) {
        mock("Corridor").actualCall("setAltitude").onObject(this).withParameter("newAltitude", newAltitude);
    }

    virtual float getAltitude(void) const {
        return (float) mock("Corridor").actualCall("getAltitude").onObject(const_cast<CorridorMock*>(this)).returnDoubleValueOrDefault(0.0);
    }

    virtual ~CorridorMock() {
        while (!checkConflictsCorridorsLists.empty()) {
            std::list<AMW_Corridor*>* corridors = checkConflictsCorridorsLists.front();
            checkConflictsCorridorsLists.pop_front();
            delete corridors;
        }

    }

    std::list<std::list<AMW_Corridor*>*> checkConflictsCorridorsLists;

    AMW_Corridor_Conflict* checkConflicts(const AMW_List<AMW_Corridor*>* corridors, bool checkFullCorridor) const {
        if (corridors) {
            std::list<AMW_Corridor*>* corridorList = new std::list<AMW_Corridor*>;
            AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
            while (iterator->hasNext()) {
                corridorList->push_back(iterator->next());
            }
            delete iterator;
            const_cast<CorridorMock*>(this)->checkConflictsCorridorsLists.push_back(corridorList);
        }


        return (AMW_Corridor_Conflict*) mock("Corridor").actualCall("checkConflicts").onObject(const_cast<CorridorMock*>(this)).withParameter("checkFullCorridor", checkFullCorridor).returnPointerValueOrDefault(0);
    }

    virtual uint8_t getId() const {
        return (uint8_t) mock("Corridor").actualCall("getId").onObject(const_cast<CorridorMock*>(this)).returnIntValueOrDefault(1);
    }
};

#endif /* STUBS_CORRIDORMOCK_H_ */
