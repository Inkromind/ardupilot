/*
 * CorridorManagerTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Corridors/AMW_Corridor_Manager.h"

//Mocks
#include  "../mocks/FacadeMock.h"
#include  "../mocks/CorridorMock.h"
#include  "../mocks/CommunicationFacadeMock.h"

#define CM_MIN_RESERVATION_TIMEOUT 30
#define CM_MAX_RESERVATION_TIMEOUT 35
#define CM_MIN_RETRY_TIMEOUT 30
#define CM_MAX_RETRY_TIMEOUT 35
#define CM_MIN_ROUND_DELAY_TIMEOUT 0
#define CM_MAX_ROUND_DELAY_TIMEOUT 5
#define CM_MIN_ALTITUDE 1000
#define CM_CORRIDOR_HEIGHT 1000

//Includes
#include <stdlib.h>

class DummyCorridorManager : public AMW_Corridor_Manager {
public:
    AMW_List<AMW_Corridor*>* getReservedCorridors() {
        return &reservedCorridors;
    }
    AMW_List<AMW_Corridor*>* getPreliminaryCorridors() {
        return &preliminaryCorridors;
    }

    uint16_t getCurrentReservationId() {
        return currentReservationId;
    }

    void setCurrentReservationId(uint8_t newId) {
        currentReservationId = newId;
    }

    uint8_t getFailures() {
        return failures;
    }

    void setMinAltitude(float newMinAltitude) {
        minAltitude = newMinAltitude;
    }

    void setFailures(uint8_t newFailures) {
        failures = newFailures;
    }

    float getReservedAltitude() {
        return reservedAltitude;
    }

    State getCurrentState() {
        return currentState;
    }
    void setCurrentState(State newState) {
        currentState = newState;
    }
    void setCorridorConflict(bool newConflict) {
        corridorConflict = newConflict;
    }

    const AMW_Module_Identifier* getReservedModule() {
        return reservedModule;
    }

    void setReservedModule(AMW_Module_Identifier* newModule) {
        reservedModule = newModule;
    }

    float getPreliminaryAltitude() {
        return preliminaryAltitude;
    }

    void setPreliminaryAltitude(float newAlt) {
        preliminaryAltitude = newAlt;
    }
    void setReservedAltitude(float newAlt) {
        reservedAltitude = newAlt;
    }
    uint32_t getWaitStart() {
        return waitStart;
    }

    void setWaitStart(uint32_t newStart) {
        waitStart = newStart;
    }
    float getWaitTimeout() {
        return waitTimeout;
    }

    void setWaitTimeout(float newTimeout) {
        waitTimeout = newTimeout;
    }

    void setFailed(bool newFailed) {
        failed = newFailed;
    }

    uint8_t getMaxFailures() {
        return maxFailures;
    }

    void setMaxFailures(uint8_t newFailures) {
        maxFailures = newFailures;
    }
};

FacadeMock* CMfacadeMock = 0;
CommunicationFacadeMock* CMcommFacadeMock = 0;
DummyCorridorManager* CMmanager = 0;
AMW_Module_Identifier* CMidentifierLowPriority = 0;
AMW_Module_Identifier* CMidentifierLowPriority2 = 0;
AMW_Module_Identifier* CMidentifierHighPriority = 0;

TEST_GROUP(CorridorManager)
{
    void setup()
    {
        srand(1);
        CMfacadeMock = new FacadeMock();
        FacadeMock::setFacade(CMfacadeMock);
        CMcommFacadeMock = new CommunicationFacadeMock();
        CommunicationFacadeMock::setFacade(CMcommFacadeMock);
        CMmanager = new DummyCorridorManager();
        CMidentifierLowPriority = new AMW_Module_Identifier(0);
        CMidentifierLowPriority2 = new AMW_Module_Identifier(0);
        CMidentifierHighPriority = new AMW_Module_Identifier(10);
    }
    void teardown()
    {
        FacadeMock::deleteFacade();
        delete CMfacadeMock;
        CommunicationFacadeMock::deleteFacade();
        delete CMcommFacadeMock;
        delete CMmanager;
        delete CMidentifierLowPriority;
        delete CMidentifierLowPriority2;
        delete CMidentifierHighPriority;
    }
};

TEST(CorridorManager, canReserveCorridorsNullModule)
{
    CorridorMock corridor;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setReservedModule(CMidentifierHighPriority);

    CHECK_FALSE(CMmanager->canReserveCorridors(0));
}
TEST(CorridorManager, canReserveCorridorsNoCorridorsReserved)
{
    CMmanager->setReservedModule(CMidentifierHighPriority);

    CHECK_TRUE(CMmanager->canReserveCorridors(CMidentifierLowPriority));
}

TEST(CorridorManager, canReserveCorridorsNoModule)
{
    CorridorMock corridor;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);

    CHECK_TRUE(CMmanager->canReserveCorridors(CMidentifierLowPriority));

}
TEST(CorridorManager, canReserveCorridorsSameModule)
{
    CorridorMock corridor;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_TRUE(CMmanager->canReserveCorridors(CMidentifierLowPriority));

}
TEST(CorridorManager, canReserveCorridorsHigherPriority)
{
    CorridorMock corridor;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_TRUE(CMmanager->canReserveCorridors(CMidentifierHighPriority));
}
TEST(CorridorManager, canReserveCorridorsEqualPriority)
{
    CorridorMock corridor;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->canReserveCorridors(CMidentifierLowPriority2));
}

TEST(CorridorManager, reserveCorridorsNullModule)
{
    CorridorMock corridor;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor);

    CHECK_FALSE(CMmanager->reserveCorridors(0, &corridors));

    CHECK_EQUAL(0, CMmanager->getReservedModule());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
}

TEST(CorridorManager, reserveCorridorsNullCorridors)
{
    CHECK_FALSE(CMmanager->reserveCorridors(CMidentifierLowPriority, 0));

    CHECK_EQUAL(0, CMmanager->getReservedModule());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
}

TEST(CorridorManager, reserveCorridorsEmptyCorridors)
{
    AMW_List<AMW_Corridor*> corridors;

    CHECK_FALSE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors));

    CHECK_EQUAL(0, CMmanager->getReservedModule());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
}

TEST(CorridorManager, reserveCorridorsAlreadyReservedByHigherPriority)
{
    CMmanager->setReservedModule(CMidentifierHighPriority);
    CorridorMock corridor;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor);

    CHECK_FALSE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors));

    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
}
TEST(CorridorManager, reserveCorridorsAlreadyReservingByHigherPriority)
{
    CMmanager->setReservedModule(CMidentifierHighPriority);
    CorridorMock corridor;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);

    CHECK_FALSE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors));

    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getPreliminaryCorridors()->front());
}

TEST(CorridorManager, reserveCorridors)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 2));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(CM_MIN_ALTITUDE, CMmanager->getPreliminaryAltitude());
}
TEST(CorridorManager, reserveCorridorsHigherPriority)
{
    CorridorMock corridor3;
    CorridorMock corridor4;
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    CMmanager->setReservedAltitude(20);
    CMmanager->setFailures(3);

    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierHighPriority, &corridors, 2));

    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_EQUAL(0, CMmanager->getReservedAltitude());
    CHECK_EQUAL(0, CMmanager->getFailures());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(CM_MIN_ALTITUDE, CMmanager->getPreliminaryAltitude());
}

TEST(CorridorManager, reserveCorridorsSameModuleNewReservation)
{
    CorridorMock corridor3;
    CorridorMock corridor4;
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor4);
    CMmanager->setReservedModule(CMidentifierHighPriority);
    CMmanager->setCurrentReservationId(1);
    CMmanager->setReservedAltitude(20);
    CMmanager->setFailures(3);

    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 2);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierHighPriority, &corridors, 2));

    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_EQUAL(20, CMmanager->getReservedAltitude());
    CHECK_EQUAL(0, CMmanager->getFailures());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(CM_MIN_ALTITUDE, CMmanager->getPreliminaryAltitude());
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
}

TEST(CorridorManager, reserveCorridorsMaxReservationId)
{
    CMmanager->setCurrentReservationId(0xff);
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 0);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 2));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
}

TEST(CorridorManager, reserveCorridorsMinAltitudeTooBig)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    CHECK_FALSE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 2, 3001));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
}
TEST(CorridorManager, reserveCorridorsMinAltitudeTooSmall)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 2, 999));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(CM_MIN_ALTITUDE, CMmanager->getPreliminaryAltitude());
}
TEST(CorridorManager, reserveCorridorsMinAltitude)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", 1001);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", 1001);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 2, 1001));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(2, CMmanager->getMaxFailures());
    CHECK_EQUAL(10, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(1001, CMmanager->getPreliminaryAltitude());
}

TEST(CorridorManager, corridorsAreReservedNullModule)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(&corridor1);

    CHECK_FALSE(CMmanager->corridorsAreReserved(0, &corridors));
}

TEST(CorridorManager, corridorsAreReservedDifferentModule)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority2);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
}

TEST(CorridorManager, corridorsAreReservedNullCorridors)
{
    CorridorMock corridor1;
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, 0));
}

TEST(CorridorManager, corridorsAreReservedEmptyCorridors)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
}

TEST(CorridorManager, corridorsAreReservedTooManyCorridors)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
}

TEST(CorridorManager, corridorsAreReservedNotAllReserved)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CorridorMock corridor4;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getReservedCorridors()->push_back(&corridor4);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor1);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
}

TEST(CorridorManager, corridorsAreReservedAllReserved)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getReservedCorridors()->push_back(&corridor2);

    CHECK_TRUE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
}

TEST(CorridorManager, isReservingCorridorsNoCorridors)
{
    CMmanager->setReservedModule(CMidentifierHighPriority);

    CHECK_FALSE(CMmanager->isReservingCorridors(CMidentifierHighPriority));
}

TEST(CorridorManager, isReservingCorridorsOtherModule)
{
    CorridorMock corridor1;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->isReservingCorridors(CMidentifierHighPriority));
}

TEST(CorridorManager, isReservingCorridorsNullModule)
{
    CorridorMock corridor1;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_FALSE(CMmanager->isReservingCorridors(0));
}

TEST(CorridorManager, isReservingCorridorsTrue)
{
    CorridorMock corridor1;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor1);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
}

TEST(CorridorManager, markCorridorsReservedNullModule)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);

    CHECK_FALSE(CMmanager->markCorridorsReserved(0, &corridors));

    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
}

TEST(CorridorManager, markCorridorsReservedNullCorridors)
{
    CHECK_FALSE(CMmanager->markCorridorsReserved(CMidentifierLowPriority, 0));

    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(0, CMmanager->getReservedModule());
}
TEST(CorridorManager, markCorridorsReservedEmptyCorridors)
{
    AMW_List<AMW_Corridor*> corridors;
    CHECK_FALSE(CMmanager->markCorridorsReserved(CMidentifierLowPriority, &corridors));
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(0, CMmanager->getReservedModule());
}
TEST(CorridorManager, markCorridorsReservedCannotReserveCorridors)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CorridorMock corridor2;
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority2);

    CHECK_FALSE(CMmanager->markCorridorsReserved(CMidentifierLowPriority, &corridors));

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(CMidentifierLowPriority2, CMmanager->getReservedModule());
}
TEST(CorridorManager, markCorridorsReservedOtherModule)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CorridorMock corridor2;
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority2);

    mock("Corridor").expectNCalls(2, "getAltitude").onObject(&corridor1).andReturnValue(10.0);

    CHECK_TRUE(CMmanager->markCorridorsReserved(CMidentifierHighPriority, &corridors));

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor1, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(10.0, CMmanager->getReservedAltitude());
}
TEST(CorridorManager, markCorridorsReservedSameModule)
{
    CorridorMock corridor1;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    CorridorMock corridor2;
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierHighPriority);
    CMmanager->setReservedAltitude(20);

    mock("Corridor").expectOneCall("getAltitude").onObject(&corridor1).andReturnValue(0.0);

    CHECK_TRUE(CMmanager->markCorridorsReserved(CMidentifierHighPriority, &corridors));

    CHECK_EQUAL(2, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(&corridor1, CMmanager->getReservedCorridors()->back());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(CMidentifierHighPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(20, CMmanager->getReservedAltitude());
}

TEST(CorridorManager, freeCorridorsNullCorridors)
{
    CMmanager->freeCorridors(0);
}

TEST(CorridorManager, freeCorridorsEmptyCorridors)
{
    AMW_List<AMW_Corridor*> corridors;
    CMmanager->freeCorridors(&corridors);
}

TEST(CorridorManager, freeCorridors)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock* corridor3 = new CorridorMock();
    CorridorMock* corridor4 = new CorridorMock();
    CorridorMock* corridor5 = new CorridorMock();
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor3);
    corridors.push_back(corridor4);
    corridors.push_back(corridor5);

    CMmanager->freeCorridors(&corridors);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor1, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getPreliminaryCorridors()->front());
    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
}

TEST(CorridorManager, freeCorridorsNoMoreReservedCorridors)
{
    CorridorMock corridor2;
    CorridorMock* corridor3 = new CorridorMock();
    CorridorMock* corridor4 = new CorridorMock();
    CorridorMock* corridor5 = new CorridorMock();
    CMmanager->getReservedCorridors()->push_back(corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    CMmanager->setCorridorConflict(true);
    CHECK_TRUE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor3);
    corridors.push_back(corridor4);
    corridors.push_back(corridor5);

    CMmanager->freeCorridors(&corridors);

    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getPreliminaryCorridors()->front());
    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_FALSE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
}

TEST(CorridorManager, freeCorridorsNoMorePreliminaryCorridors)
{
    CorridorMock corridor1;
    CorridorMock* corridor3 = new CorridorMock();
    CorridorMock* corridor4 = new CorridorMock();
    CorridorMock* corridor5 = new CorridorMock();
    CMmanager->getReservedCorridors()->push_back(&corridor1);
    CMmanager->getReservedCorridors()->push_back(corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor3);
    corridors.push_back(corridor4);
    corridors.push_back(corridor5);

    CMmanager->freeCorridors(&corridors);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor1, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
}

TEST(CorridorManager, freeCorridorsNoMoreCorridors)
{
    CorridorMock* corridor3 = new CorridorMock();
    CorridorMock* corridor4 = new CorridorMock();
    CorridorMock* corridor5 = new CorridorMock();
    CMmanager->getReservedCorridors()->push_back(corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor3);
    corridors.push_back(corridor4);
    corridors.push_back(corridor5);

    CMmanager->freeCorridors(&corridors);

    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(0, CMmanager->getReservedModule());
}

TEST(CorridorManager, checkReservationRequestNullCorridors)
{
    CHECK_EQUAL(0, CMmanager->checkReservationRequest(0));
}

TEST(CorridorManager, checkReservationRequestEmptyCorridors)
{
    AMW_List<AMW_Corridor*> corridors;

    CHECK_EQUAL(0, CMmanager->checkReservationRequest(&corridors));
}
TEST(CorridorManager, checkReservationRequestConflictWithPosition)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor3);
    Vector3f position = Vector3f(10, 10, 10);
    Vector3f position2 = Vector3f(10, 10, 310);
    Vector3f position3 = Vector3f(10, 10, 309);

    mock("Facade").expectOneCall("getRealPosition").onObject(CMfacadeMock).andReturnValue(&position);
    mock("Corridor").expectOneCall("getType").onObject(&corridor1).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&corridor1).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectNCalls(2, "getType").onObject(&corridor2).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&corridor2).withParameter("checkFullCorridor", true).andReturnValue(&position3);
    mock("Corridor").expectOneCall("getId").onObject(&corridor2).andReturnValue(5);
    mock("Corridor").expectOneCall("getAltitude").onObject(&corridor2).andReturnValue(309.0);

    AMW_Corridor_Conflict* conflict = CMmanager->checkReservationRequest(&corridors);

    CHECK_TRUE(conflict);
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOwnType())
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOtherType())
    CHECK_EQUAL(5, conflict->getOtherId())
    CHECK_EQUAL(309, conflict->getOtherAltitude())
    delete conflict;
}
TEST(CorridorManager, checkReservationRequestNoConflictWithPosition)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor3);
    Vector3f position = Vector3f(10, 10, 10);
    Vector3f position2 = Vector3f(10, 10, 310);
    Vector3f position3 = Vector3f(10, 10, 311);

    mock("Facade").expectOneCall("getRealPosition").onObject(CMfacadeMock).andReturnValue(&position);
    mock("Corridor").expectOneCall("getType").onObject(&corridor1).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&corridor1).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getType").onObject(&corridor2).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&corridor2).withParameter("checkFullCorridor", true).andReturnValue(&position3);

    CHECK_EQUAL(0, CMmanager->checkReservationRequest(&corridors));
}
TEST(CorridorManager, checkReservationRequestConflictWithReservedCorridor)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CorridorMock corridor4;
    CorridorMock corridor5;
    CorridorMock corridor6;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor3);
    CMmanager->getReservedCorridors()->push_back(&corridor4);
    CMmanager->getReservedCorridors()->push_back(&corridor5);
    CMmanager->getReservedCorridors()->push_back(&corridor6);
    AMW_Corridor_Conflict conflict(0, 0, 0, 0, 0, 0);

    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor4).withParameter("checkFullCorridor", true).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor5).withParameter("checkFullCorridor", true).andReturnValue(&conflict);

    CHECK_EQUAL(&conflict, CMmanager->checkReservationRequest(&corridors));

    CHECK_EQUAL(1, (int) corridor4.checkConflictsCorridorsLists.size());
    std::list<AMW_Corridor*>* list = corridor4.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
    CHECK_EQUAL(1, (int) corridor5.checkConflictsCorridorsLists.size());
    list = corridor5.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
}

TEST(CorridorManager, checkReservationRequestConflictWithPreliminaryCorridor)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CorridorMock corridor4;
    CorridorMock corridor5;
    CorridorMock corridor6;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor4);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor5);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor6);
    AMW_Corridor_Conflict conflict(0, 0, 0, 0, 0, 0);

    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor3).withParameter("checkFullCorridor", true).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor4).withParameter("checkFullCorridor", true).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor5).withParameter("checkFullCorridor", true).andReturnValue(&conflict);

    CHECK_EQUAL(&conflict, CMmanager->checkReservationRequest(&corridors));

    CHECK_EQUAL(1, (int) corridor3.checkConflictsCorridorsLists.size());
    std::list<AMW_Corridor*>* list = corridor3.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
    CHECK_EQUAL(1, (int) corridor4.checkConflictsCorridorsLists.size());
    list = corridor4.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
    CHECK_EQUAL(1, (int) corridor5.checkConflictsCorridorsLists.size());
    list = corridor5.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
}
TEST(CorridorManager, checkReservationRequestNoConflict)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CorridorMock corridor4;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor4);

    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor3).withParameter("checkFullCorridor", true).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor4).withParameter("checkFullCorridor", true).andReturnValue((AMW_Corridor_Conflict*) 0);

    CHECK_EQUAL(0, CMmanager->checkReservationRequest(&corridors));

    CHECK_EQUAL(1, (int) corridor3.checkConflictsCorridorsLists.size());
    std::list<AMW_Corridor*>* list = corridor3.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
    CHECK_EQUAL(1, (int) corridor4.checkConflictsCorridorsLists.size());
    list = corridor4.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) list->size());
    CHECK_EQUAL(&corridor1, list->front());
    CHECK_EQUAL(&corridor2, list->back());
}


TEST(CorridorManager, checkTimeoutNoPreliminaryCorridors)
{
    CMmanager->setFailures(2);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);

    CMmanager->checkTimeout();

    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}
TEST(CorridorManager, checkTimeoutNotReserving)
{
    CorridorMock corridor;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setFailures(2);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::IDLE);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getPreliminaryCorridors()->front());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(AMW_Corridor_Manager::State::IDLE, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}
TEST(CorridorManager, checkTimeoutRequestSendWaitNotPassed)
{
    CorridorMock corridor;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setFailures(2);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(15 * 1000);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getPreliminaryCorridors()->front());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}
TEST(CorridorManager, checkTimeoutWaitingForRetryWaitNotPassed)
{
    CorridorMock corridor;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setFailures(2);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::WAITING_FOR_RETRY);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(15 * 1000);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getPreliminaryCorridors()->front());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_RETRY, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}
TEST(CorridorManager, checkTimeoutWaitingForNextRoundWaitNotPassed)
{
    CorridorMock corridor;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->setFailures(2);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(15 * 1000);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(&corridor, CMmanager->getPreliminaryCorridors()->front());
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}
TEST(CorridorManager, checkTimeoutRequestSendReserveCorridors)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->setFailures(2);
    CMmanager->setPreliminaryAltitude(200);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);

    CMmanager->checkTimeout();

    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(3, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor3, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->back());
    CHECK_EQUAL(&corridor, CMmanager->getReservedCorridors()->get(1));
    CHECK_EQUAL(AMW_Corridor_Manager::State::IDLE, CMmanager->getCurrentState());
    CHECK_EQUAL(2, CMmanager->getFailures());
}

TEST(CorridorManager, checkTimeoutRequestWaitingForRetryRetry)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->setFailures(2);
    CMmanager->setCurrentReservationId(5);
    CMmanager->setPreliminaryAltitude(600);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::WAITING_FOR_RETRY);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);
    CMmanager->setMinAltitude(1010);

    mock("Facade").expectNCalls(2, "getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);
    mock("Corridor").expectOneCall("setAltitude").onObject(&corridor).withDoubleParameter("newAltitude", 1010);
    mock("Corridor").expectOneCall("setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", 1010);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 6);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1010, CMmanager->getPreliminaryAltitude());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor3, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
}

TEST(CorridorManager, checkTimeoutRequestWaitingForNextRoundStartNextRound)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->setFailures(2);
    CMmanager->setCurrentReservationId(5);
    CMmanager->setPreliminaryAltitude(600);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND);
    CMmanager->setWaitStart(10 * 1000);
    CMmanager->setWaitTimeout(5);

    mock("Facade").expectNCalls(2, "getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);
    mock("Corridor").expectOneCall("setAltitude").onObject(&corridor).withDoubleParameter("newAltitude", 600);
    mock("Corridor").expectOneCall("setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", 600);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 6);

    CMmanager->checkTimeout();

    CHECK_EQUAL(600, CMmanager->getPreliminaryAltitude());
    CHECK_EQUAL(2, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor3, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RESERVATION_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RESERVATION_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());

}


TEST(CorridorManager, reservationConflictReceivedNullConflict)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);

    CMmanager->reservationConflictReceived(10, 0);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
}

TEST(CorridorManager, reservationConflictReceivedWrongReservationId)
{
    AMW_Corridor_Conflict conflict(0, 0, 0, 0, 0, 0);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);

    CMmanager->reservationConflictReceived(9, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(AMW_Corridor_Manager::State::REQUEST_SEND, CMmanager->getCurrentState());
}


TEST(CorridorManager, reservationConflictReceivedNotWaitingForReply)
{
    AMW_Corridor_Conflict conflict(0, 0, 0, 0, 0, 0);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND, CMmanager->getCurrentState());
}
TEST(CorridorManager, reservationConflictReceivedOwnCorridorVertical)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::VERTICAL, 0, 0, 0, 0, 0);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(5);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(3, CMmanager->getFailures());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RETRY_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RETRY_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_RETRY, CMmanager->getCurrentState());
}

TEST(CorridorManager, reservationConflictReceivedOwnCorridorVerticalMaxFailuresReached)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::VERTICAL, 0, 0, 0, 0, 0);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(3);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(3, CMmanager->getFailures());
    CHECK_TRUE(CMmanager->reservationHasFailed(CMidentifierLowPriority))
    CHECK_EQUAL(AMW_Corridor_Manager::State::IDLE, CMmanager->getCurrentState());
}
TEST(CorridorManager, reservationConflictReceivedOtherCorridorVertical)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::HORIZONTAL, 0, 0, AMW_Corridor::Type::VERTICAL, 0, 2000);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(5);
    CMmanager->setPreliminaryAltitude(500);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_EQUAL(3000, CMmanager->getPreliminaryAltitude());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_ROUND_DELAY_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_ROUND_DELAY_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND, CMmanager->getCurrentState());
}
TEST(CorridorManager, reservationConflictReceivedNextRound)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::HORIZONTAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 2000);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(5);
    CMmanager->setPreliminaryAltitude(500);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(2, CMmanager->getFailures());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_EQUAL(1500, CMmanager->getPreliminaryAltitude());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_ROUND_DELAY_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_ROUND_DELAY_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_NEXT_ROUND, CMmanager->getCurrentState());
}
TEST(CorridorManager, reservationConflictReceivedAltitudeLimitReached)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::HORIZONTAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 2000);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(5);
    CMmanager->setPreliminaryAltitude(2501);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(16 * 1000);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_EQUAL(1, CMmanager->getPreliminaryCorridors()->size());
    CHECK_EQUAL(3, CMmanager->getFailures());
    CHECK_EQUAL(16 * 1000, CMmanager->getWaitStart());
    CHECK_TRUE(CMmanager->getWaitTimeout() >= CM_MIN_RETRY_TIMEOUT && CMmanager->getWaitTimeout() <= CM_MAX_RETRY_TIMEOUT);
    CHECK_EQUAL(AMW_Corridor_Manager::State::WAITING_FOR_RETRY, CMmanager->getCurrentState());
}
TEST(CorridorManager, reservationConflictReceivedAltitudeLimitReachedMaxFailuresReached)
{
    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::HORIZONTAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 2000);
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->setCurrentReservationId(10);
    CMmanager->setCurrentState(AMW_Corridor_Manager::State::REQUEST_SEND);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setFailures(2);
    CMmanager->setMaxFailures(3);
    CMmanager->setPreliminaryAltitude(2501);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CMmanager->reservationConflictReceived(10, &conflict);

    CHECK_EQUAL(1, CMmanager->getReservedCorridors()->size());
    CHECK_EQUAL(&corridor2, CMmanager->getReservedCorridors()->front());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_EQUAL(3, CMmanager->getFailures());
    CHECK_TRUE(CMmanager->reservationHasFailed(CMidentifierLowPriority))
    CHECK_EQUAL(AMW_Corridor_Manager::State::IDLE, CMmanager->getCurrentState());
}

TEST(CorridorManager, broadcastReservedCorridorsNoReservedCorridors)
{
    CorridorMock corridor;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    Vector3f position = Vector3f(10, 11, 12);

    mock("Facade").expectOneCall("getRealPosition").onObject(CMfacadeMock).andReturnValue(&position);
    mock("ComFacade").expectOneCall("broadcastCorridors").onObject(CMcommFacadeMock);

    CMmanager->broadcastReservedCorridors();

    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastCorridorsLists.size());
    std::list<AMW_Corridor*>* corridors = CMcommFacadeMock->broadcastCorridorsLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    CHECK_FALSE(corridors->front() == &corridor);
    // We can't check the position corridor here because it has already been deleted.
}
TEST(CorridorManager, broadcastReservedCorridorsReservedCorridors)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CMmanager->getReservedCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->getPreliminaryCorridors()->push_back(&corridor3);

    mock("ComFacade").expectOneCall("broadcastCorridors").onObject(CMcommFacadeMock);

    CMmanager->broadcastReservedCorridors();

    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastCorridorsLists.size());
    std::list<AMW_Corridor*>* corridors = CMcommFacadeMock->broadcastCorridorsLists.front();
    CHECK_EQUAL(2, (int) corridors->size());
    CHECK_EQUAL(&corridor, corridors->front());
    CHECK_EQUAL(&corridor2, corridors->back());
}

TEST(CorridorManager, receivedCorridorBroadcastNullCorridors)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority);

    CMmanager->receivedCorridorBroadcast(0);

    CHECK_FALSE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
}

TEST(CorridorManager, receivedCorridorBroadcastEmptyCorridors)
{
    CorridorMock corridor;
    CorridorMock corridor2;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;

    CMmanager->receivedCorridorBroadcast(&corridors);

    CHECK_FALSE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
}

TEST(CorridorManager, receivedCorridorBroadcastConflictWithReserved)
{
    CorridorMock corridor;
    CorridorMock corridor5;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CorridorMock corridor4;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->getReservedCorridors()->push_back(&corridor4);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor);
    corridors.push_back(&corridor5);
    AMW_Corridor_Conflict* conflict = new AMW_Corridor_Conflict(0, 0, 0, 0, 0, 0);

    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor2).withParameter("checkFullCorridor", false).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor3).withParameter("checkFullCorridor", false).andReturnValue(conflict);

    CMmanager->receivedCorridorBroadcast(&corridors);

    CHECK_TRUE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
    CHECK_EQUAL(1, (int) corridor2.checkConflictsCorridorsLists.size());
    std::list<AMW_Corridor*>* conflictCorridors = corridor2.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) conflictCorridors->size());
    CHECK_EQUAL(&corridor, conflictCorridors->front());
    CHECK_EQUAL(&corridor5, conflictCorridors->back());
    CHECK_EQUAL(1, (int) corridor3.checkConflictsCorridorsLists.size());
    conflictCorridors = corridor3.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) conflictCorridors->size());
    CHECK_EQUAL(&corridor, conflictCorridors->front());
    CHECK_EQUAL(&corridor5, conflictCorridors->back());
}

TEST(CorridorManager, receivedCorridorBroadcastNoConflict)
{
    CorridorMock corridor;
    CorridorMock corridor5;
    CorridorMock corridor2;
    CorridorMock corridor3;
    CMmanager->getPreliminaryCorridors()->push_back(&corridor);
    CMmanager->getReservedCorridors()->push_back(&corridor2);
    CMmanager->getReservedCorridors()->push_back(&corridor3);
    CMmanager->setReservedModule(CMidentifierLowPriority);
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor);
    corridors.push_back(&corridor5);

    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor2).withParameter("checkFullCorridor", false).andReturnValue((AMW_Corridor_Conflict*) 0);
    mock("Corridor").expectOneCall("checkConflicts").onObject(&corridor3).withParameter("checkFullCorridor", false).andReturnValue((AMW_Corridor_Conflict*) 0);

    CMmanager->receivedCorridorBroadcast(&corridors);

    CHECK_FALSE(CMmanager->hasCorridorConflict(CMidentifierLowPriority));
    CHECK_EQUAL(1, (int) corridor2.checkConflictsCorridorsLists.size());
    std::list<AMW_Corridor*>* conflictCorridors = corridor2.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) conflictCorridors->size());
    CHECK_EQUAL(&corridor, conflictCorridors->front());
    CHECK_EQUAL(&corridor5, conflictCorridors->back());
    CHECK_EQUAL(1, (int) corridor3.checkConflictsCorridorsLists.size());
    conflictCorridors = corridor3.checkConflictsCorridorsLists.front();
    CHECK_EQUAL(2, (int) conflictCorridors->size());
    CHECK_EQUAL(&corridor, conflictCorridors->front());
    CHECK_EQUAL(&corridor5, conflictCorridors->back());
}

TEST(CorridorManager, ScenarioTestSuccesfullReservation)
{
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor1);
    corridors.push_back(corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10 * 1000);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 3));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CMcommFacadeMock->broadcastReservationRequestLists.pop_front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(corridor2, broadcastedCorridors->back());
    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
    delete broadcastedCorridors;

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10 * 1000 + CM_MIN_RESERVATION_TIMEOUT * 1000 );

    CMmanager->checkTimeout();

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors))
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));

    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::HORIZONTAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 1000);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(30 * 1000);

    CMmanager->reservationConflictReceived(1, &conflict);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));


    mock("Facade").expectNCalls(2, "getTimeMillis").onObject(CMfacadeMock).andReturnValue(40 * 1000);
    mock("Corridor").expectOneCall("setAltitude").onObject(corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE + CM_CORRIDOR_HEIGHT);
    mock("Corridor").expectOneCall("setAltitude").onObject(corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE + CM_CORRIDOR_HEIGHT);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 2);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CMcommFacadeMock->broadcastReservationRequestLists.pop_front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(corridor2, broadcastedCorridors->back());
    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
    delete broadcastedCorridors;

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(100 * 1000);

    CMmanager->checkTimeout();

    CHECK_TRUE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_EQUAL(CM_MIN_ALTITUDE + CM_CORRIDOR_HEIGHT, CMmanager->getReservedAltitude());

    CMmanager->freeCorridors(&corridors);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_TRUE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_FALSE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
}

TEST(CorridorManager, ScenarioTestFailedReservation)
{
    CorridorMock corridor1;
    CorridorMock corridor2;
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(&corridor1);
    corridors.push_back(&corridor2);

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor1).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(&corridor2).withDoubleParameter("newAltitude", CM_MIN_ALTITUDE);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10 * 1000);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 1));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CMcommFacadeMock->broadcastReservationRequestLists.pop_front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(&corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(&corridor2, broadcastedCorridors->back());
    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
    delete broadcastedCorridors;

    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::VERTICAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 2000);

    CMmanager->reservationConflictReceived(1, &conflict);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_TRUE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_FALSE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
}
TEST(CorridorManager, ScenarioTestSuccesfullReservationWithRetry)
{
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_back(corridor1);
    corridors.push_back(corridor2);
    float minAltitude = 1750;

    mock("Corridor").expectNCalls(2, "setAltitude").onObject(corridor1).withDoubleParameter("newAltitude", minAltitude);
    mock("Corridor").expectNCalls(2, "setAltitude").onObject(corridor2).withDoubleParameter("newAltitude", minAltitude);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10 * 1000);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 1);

    CHECK_TRUE(CMmanager->reserveCorridors(CMidentifierLowPriority, &corridors, 3, minAltitude));

    CHECK_EQUAL(CMidentifierLowPriority, CMmanager->getReservedModule());
    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    std::list<AMW_Corridor*>* broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CMcommFacadeMock->broadcastReservationRequestLists.pop_front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(corridor2, broadcastedCorridors->back());
    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
    delete broadcastedCorridors;

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(10 * 1000 + CM_MIN_RESERVATION_TIMEOUT * 1000 );

    CMmanager->checkTimeout();

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors))
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));

    AMW_Corridor_Conflict conflict(AMW_Corridor::Type::VERTICAL, 0, 0, AMW_Corridor::Type::HORIZONTAL, 0, 1000);

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(30 * 1000);

    CMmanager->reservationConflictReceived(1, &conflict);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));

    mock("Facade").expectNCalls(2, "getTimeMillis").onObject(CMfacadeMock).andReturnValue(80 * 1000);
    mock("Corridor").expectOneCall("setAltitude").onObject(corridor1).withDoubleParameter("newAltitude", minAltitude);
    mock("Corridor").expectOneCall("setAltitude").onObject(corridor2).withDoubleParameter("newAltitude", minAltitude);
    mock("ComFacade").expectOneCall("broadcastReservationRequest").onObject(CMcommFacadeMock).withParameter("reservationId", 2);

    CMmanager->checkTimeout();

    CHECK_EQUAL(1, (int) CMcommFacadeMock->broadcastReservationRequestLists.size());
    broadcastedCorridors = CMcommFacadeMock->broadcastReservationRequestLists.front();
    CMcommFacadeMock->broadcastReservationRequestLists.pop_front();
    CHECK_EQUAL(2, (int) broadcastedCorridors->size());
    CHECK_EQUAL(corridor1, broadcastedCorridors->front());
    CHECK_EQUAL(corridor2, broadcastedCorridors->back());
    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_TRUE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
    delete broadcastedCorridors;

    mock("Facade").expectOneCall("getTimeMillis").onObject(CMfacadeMock).andReturnValue(120 * 1000);

    CMmanager->checkTimeout();

    CHECK_TRUE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_FALSE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_EQUAL(minAltitude, CMmanager->getReservedAltitude());

    CMmanager->freeCorridors(&corridors);

    CHECK_FALSE(CMmanager->corridorsAreReserved(CMidentifierLowPriority, &corridors));
    CHECK_TRUE(CMmanager->getReservedCorridors()->empty());
    CHECK_TRUE(CMmanager->getPreliminaryCorridors()->empty());
    CHECK_TRUE(CMmanager->reservationHasFailed(CMidentifierLowPriority));
    CHECK_FALSE(CMmanager->isReservingCorridors(CMidentifierLowPriority));
}
