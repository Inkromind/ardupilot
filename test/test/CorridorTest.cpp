/*
 * CorridorTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Corridors/AMW_Corridor.h"

//Mocks
#include "../mocks/CorridorMock.h"

//Includes
#include <AMW_List.h>
#include <stdint.h>

DummyCorridor* Ccorridor = 0;

TEST_GROUP(Corridor)
{
    void setup()
    {
        Ccorridor = new DummyCorridor(0);
        AMW_Corridor::nextId = 0;
    }
    void teardown()
    {
        delete Ccorridor;
        Ccorridor = 0;
    }
};

TEST(Corridor, ConstructorTest)
{
    DummyCorridor newCorridor;

    CHECK_EQUAL(0, newCorridor.getAltitude());
    CHECK_EQUAL(0, newCorridor.getId());

    DummyCorridor newCorridor2;

    CHECK_EQUAL(0, newCorridor2.getAltitude());
    CHECK_EQUAL(1, newCorridor2.getId());
}

TEST(Corridor, ConstructorIdTest) {
    DummyCorridor newCorridor(0);

    CHECK_EQUAL(0, newCorridor.getAltitude());
    CHECK_EQUAL(0, newCorridor.getId());

    DummyCorridor newCorridor2;

    CHECK_EQUAL(0, newCorridor2.getAltitude());
    CHECK_EQUAL(0, newCorridor2.getId());
}

TEST(Corridor, ConstructorMaxId)
{
    AMW_Corridor::nextId = 0xff;

    DummyCorridor newCorridor;

    CHECK_EQUAL(0, newCorridor.getAltitude());
    CHECK_EQUAL(0xff, newCorridor.getId());

    DummyCorridor newCorridor2;

    CHECK_EQUAL(0, newCorridor2.getAltitude());
    CHECK_EQUAL(0, newCorridor2.getId());
}

TEST(Corridor, CheckConflictsNullCorridors) {
    CHECK_EQUAL(0, Ccorridor->checkConflicts(0, true));
}

TEST(Corridor, CheckConflictsDontCheck) {
    AMW_List<AMW_Corridor*> corridors;

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(false);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(false);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

TEST(Corridor, CheckConflictsEmptyList) {
    AMW_List<AMW_Corridor*> corridors;
    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsNullCorridor) {
    AMW_List<AMW_Corridor*> corridors;
    corridors.push_front(0);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

// Position <-> Position

TEST(Corridor, CheckConflictsPositionCorridorPositionCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(10,10,10);
    Vector3f position2 = Vector3f(10,10,309);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(309.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(309, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsPositionCorridorPositionCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(10,10,10);
    Vector3f position2 = Vector3f(10,10,310);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Position <-> Horizontal

TEST(Corridor, CheckConflictsPositionCorridorHorizontalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(0);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,600,0);
    Vector3f position2a = Vector3f(0,0,299);
    Vector3f position2b = Vector3f(0,900,299);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(4, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(299.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(0, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(299, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsPositionCorridorHorizontalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,600,0);
    Vector3f position2a = Vector3f(0,0,300);
    Vector3f position2b = Vector3f(0,900,300);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsPositionCorridorHorizontalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,600,0);
    Vector3f position2a = Vector3f(0,0,300);
    Vector3f position2b = Vector3f(0,900,300);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Position <-> Vertical

TEST(Corridor, CheckConflictsPositionCorridorVerticalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(600);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,0,600);
    Vector3f position2a = Vector3f(0,299,0);
    Vector3f position2b = Vector3f(0,299,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(4, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(900.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(600, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(900, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsPositionCorridorVerticalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,0,600);
    Vector3f position2a = Vector3f(0,300,0);
    Vector3f position2b = Vector3f(0,300,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsPositionCorridorVerticalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1 = Vector3f(0,0,600);
    Vector3f position2a = Vector3f(0,300,0);
    Vector3f position2b = Vector3f(0,300,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Horizontal <-> Position

TEST(Corridor, CheckConflictsHorizontalCorridorPositionCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(299);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,600,0);
    Vector3f position1a = Vector3f(0,0,299);
    Vector3f position1b = Vector3f(0,900,299);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(4, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(0.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(299, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(0, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsHorizontalCorridorPositionCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,600,0);
    Vector3f position1a = Vector3f(0,0,300);
    Vector3f position1b = Vector3f(0,900,300);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsHorizontalCorridorPositionCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,600,0);
    Vector3f position1a = Vector3f(0,0,300);
    Vector3f position1b = Vector3f(0,900,300);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Vertical <-> Position

TEST(Corridor, CheckConflictsVerticalCorridorPositionCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(900);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,0,600);
    Vector3f position1a = Vector3f(0,299,0);
    Vector3f position1b = Vector3f(0,299,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(4, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(600.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(900, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(600, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsVerticalCorridorPositionCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,0,600);
    Vector3f position1a = Vector3f(0,300,0);
    Vector3f position1b = Vector3f(0,300,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsVerticalCorridorPositionCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position2 = Vector3f(0,0,600);
    Vector3f position1a = Vector3f(0,300,0);
    Vector3f position1b = Vector3f(0,300,900);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::POSITION);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Horizontal <-> Horizontal

TEST(Corridor, CheckConflictsHorizontalCorridorHorizontalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(299,5,10);
    Vector3f position2b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsHorizontalCorridorHorizontalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsHorizontalCorridorHorizontalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Horizontal <-> Vertical

TEST(Corridor, CheckConflictsHorizontalCorridorVerticalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(299,5,10);
    Vector3f position2b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsHorizontalCorridorVerticalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsHorizontalCorridorVerticalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Vertical <-> Vertical

TEST(Corridor, CheckConflictsVerticalCorridorVerticalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(299,5,10);
    Vector3f position2b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsVerticalCorridorVerticalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsVerticalCorridorVerticalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Vertical <-> Horizontal

TEST(Corridor, CheckConflictsVerticalCorridorHorizontalCorridorConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(299,5,10);
    Vector3f position2b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsVerticalCorridorHorizontalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}

TEST(Corridor, CheckConflictsVerticalCorridorHorizontalCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(true);
    mock("Corridor").expectOneCall("getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", false).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, false));
}

// Multiple Corridors

TEST(Corridor, CheckConflictsVerticalCorridorMultipleHorizontalCorridorFirstConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    CorridorMock dummyCorridor2;
    corridors.push_front(&dummyCorridor2);
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(299,5,10);
    Vector3f position2b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsVerticalCorridorMultipleHorizontalCorridorLastConflict) {
    AMW_List<AMW_Corridor*> corridors;
    Ccorridor->setAltitude(10);
    CorridorMock dummyCorridor;
    CorridorMock dummyCorridor2;
    corridors.push_front(&dummyCorridor2);
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);
    Vector3f position3a = Vector3f(299,5,10);
    Vector3f position3b = Vector3f(299,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(3, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectNCalls(2, "getType").onObject(&dummyCorridor2).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor2).withParameter("checkFullCorridor", true).andReturnValue(&position3a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor2).andReturnValue(&position3b);
    mock("Corridor").expectOneCall("getId").onObject(&dummyCorridor2).andReturnValue(2);
    mock("Corridor").expectOneCall("getAltitude").onObject(&dummyCorridor2).andReturnValue(10.0);

    AMW_Corridor_Conflict* conflict = Ccorridor->checkConflicts(&corridors, true);

    CHECK_TRUE(conflict);
    CHECK_EQUAL(10, conflict->getOwnAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, conflict->getOwnType())
    CHECK_EQUAL(0, conflict->getOwnId())
    CHECK_EQUAL(10, conflict->getOtherAltitude())
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, conflict->getOtherType())
    CHECK_EQUAL(2, conflict->getOtherId())
    delete conflict;
}

TEST(Corridor, CheckConflictsVerticalCorridorMultipleHorizontalCorridorCheckFullCorridorNoConflict) {
    AMW_List<AMW_Corridor*> corridors;
    CorridorMock dummyCorridor;
    CorridorMock dummyCorridor2;
    corridors.push_front(&dummyCorridor2);
    corridors.push_front(&dummyCorridor);
    Vector3f position1a = Vector3f(0,0,10);
    Vector3f position1b = Vector3f(0,10,10);
    Vector3f position2a = Vector3f(300,5,10);
    Vector3f position2b = Vector3f(300,10,10);

    mock("Corridor").expectOneCall("checkForConflicts").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(true);
    mock("Corridor").expectNCalls(2, "getType").onObject(Ccorridor).andReturnValue((uint8_t) AMW_Corridor::Type::VERTICAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getType").onObject(&dummyCorridor2).andReturnValue((uint8_t) AMW_Corridor::Type::HORIZONTAL);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor).andReturnValue(&position2b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(Ccorridor).withParameter("checkFullCorridor", true).andReturnValue(&position1a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(Ccorridor).andReturnValue(&position1b);
    mock("Corridor").expectOneCall("getStartPoint").onObject(&dummyCorridor2).withParameter("checkFullCorridor", true).andReturnValue(&position2a);
    mock("Corridor").expectOneCall("getEndPoint").onObject(&dummyCorridor2).andReturnValue(&position2b);

    CHECK_EQUAL(0, Ccorridor->checkConflicts(&corridors, true));
}
