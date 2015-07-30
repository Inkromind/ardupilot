/*
 * CorridorsTest.cpp
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"

//Class being tested
#include "../../libraries/AMW_Corridors/AMW_Position_Corridor.h"
#include "../../libraries/AMW_Corridors/AMW_Horizontal_Corridor.h"
#include "../../libraries/AMW_Corridors/AMW_Vertical_Corridor.h"

//Mocks
#include "../mocks/FacadeMock.h"

//Includes
#include <AMW_List.h>
#include <stdint.h>

FacadeMock* CsfacadeMock = 0;

TEST_GROUP(Corridors)
{
    void setup()
    {
        AMW_Corridor::nextId = 1;
        CsfacadeMock = new FacadeMock();
        FacadeMock::setFacade(CsfacadeMock);
    }
    void teardown()
    {
        FacadeMock::deleteFacade();
        delete CsfacadeMock;
        CsfacadeMock = 0;
    }
};

TEST(Corridors, PositionCorridorConstructor)
{
    AMW_Position_Corridor corridor(Vector3f(10,20,30));

    CHECK_EQUAL(1, corridor.getId());
    CHECK_EQUAL(30, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, (uint8_t) corridor.getType());
}

TEST(Corridors, PositionCorridorConstructorWithId)
{
    AMW_Position_Corridor corridor(Vector3f(10,20,30), 3);

    CHECK_EQUAL(3, corridor.getId());
    CHECK_EQUAL(30, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::POSITION, (uint8_t) corridor.getType());
}

TEST(Corridors, PositionCorridorGetPointsFullCorridors) {
    AMW_Position_Corridor corridor(Vector3f(10,20,30));

    Vector3f startPoint = corridor.getStartPoint(true);
    Vector3f endPoint = corridor.getEndPoint();

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(30, startPoint.z);
    CHECK_EQUAL(10, endPoint.x);
    CHECK_EQUAL(20, endPoint.y);
    CHECK_EQUAL(30, endPoint.z);
}

TEST(Corridors, PositionCorridorGetPointsNoFullCorridors) {
    AMW_Position_Corridor corridor(Vector3f(10,20,30));

    Vector3f startPoint = corridor.getStartPoint(false);
    Vector3f endPoint = corridor.getEndPoint();

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(30, startPoint.z);
    CHECK_EQUAL(10, endPoint.x);
    CHECK_EQUAL(20, endPoint.y);
    CHECK_EQUAL(30, endPoint.z);
}

TEST(Corridors, HorizontalCorridorConstructor)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40));

    CHECK_EQUAL(1, corridor.getId());
    CHECK_EQUAL(0, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor.getType());
}

TEST(Corridors, HorizontalCorridorConstructorWithIdAndAltitude)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    CHECK_EQUAL(6, corridor.getId());
    CHECK_EQUAL(3, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor.getType());
}

TEST(Corridors, HorizontalCorridorStartPointFullCorridorNormalDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    Vector3f startPoint = corridor.getStartPoint(true);

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorStartPointFullCorridorReverseDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);
    corridor.setReverseDirection(true);

    Vector3f startPoint = corridor.getStartPoint(true);

    CHECK_EQUAL(30, startPoint.x);
    CHECK_EQUAL(40, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorStartPointNoFullCorridorNotInCorridorNormalDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    Vector3f startPoint = corridor.getStartPoint(false);

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorStartPointNoFullCorridorNotInCorridorReverseDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);
    corridor.setReverseDirection(true);

    Vector3f startPoint = corridor.getStartPoint(false);

    CHECK_EQUAL(30, startPoint.x);
    CHECK_EQUAL(40, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorStartPointNoFullCorridorInCorridorNormalDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);
    Vector3f position(50,60,70);
    corridor.setInCorridor(true);

    mock("Facade").expectOneCall("getRealPosition").onObject(CsfacadeMock).andReturnValue(&position);

    Vector3f startPoint = corridor.getStartPoint(false);

    CHECK_EQUAL(50, startPoint.x);
    CHECK_EQUAL(60, startPoint.y);
    CHECK_EQUAL(70, startPoint.z);
}

TEST(Corridors, HorizontalCorridorEndPointNormalDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    Vector3f startPoint = corridor.getEndPoint();

    CHECK_EQUAL(30, startPoint.x);
    CHECK_EQUAL(40, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorEndPointReverseDirection)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);
    corridor.setReverseDirection(true);

    Vector3f startPoint = corridor.getEndPoint();

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(3, startPoint.z);
}

TEST(Corridors, HorizontalCorridorCheckForConflictsCheckFullCorridor)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    CHECK_TRUE(corridor.checkForConflicts(true));
}

TEST(Corridors, HorizontalCorridorCheckForConflictsNoCheckFullCorridorNotCompleted)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);

    CHECK_TRUE(corridor.checkForConflicts(false));
}

TEST(Corridors, HorizontalCorridorCheckForConflictsNoCheckFullCorridorCompleted)
{
    AMW_Horizontal_Corridor corridor(Vector2f(10,20), Vector2f(30,40), 3, 6);
    corridor.setCompleted(true);

    CHECK_FALSE(corridor.checkForConflicts(false));
}

TEST(Corridors, VerticalCorridorConstructor) {
    AMW_Vertical_Corridor corridor(Vector2f(10,20));

    CHECK_EQUAL(1, corridor.getId());
    CHECK_EQUAL(0, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor.getType());

    corridor.setAltitude(30);

    Vector3f startPoint = corridor.getStartPoint(true);
    Vector3f endPoint = corridor.getEndPoint();

    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(0, startPoint.z);
    CHECK_EQUAL(10, endPoint.x);
    CHECK_EQUAL(20, endPoint.y);
    CHECK_EQUAL(30, endPoint.z);
}

TEST(Corridors, VerticalCorridorConstructorWithAltitude) {
    AMW_Vertical_Corridor corridor(Vector2f(10,20), 30);

    CHECK_EQUAL(1, corridor.getId());
    CHECK_EQUAL(30, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor.getType());
    Vector3f startPoint = corridor.getStartPoint(true);
    Vector3f endPoint = corridor.getEndPoint();
    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(0, startPoint.z);
    CHECK_EQUAL(10, endPoint.x);
    CHECK_EQUAL(20, endPoint.y);
    CHECK_EQUAL(30, endPoint.z);
}

TEST(Corridors, VerticalCorridorConstructorWithAltitudeAndId) {
    AMW_Vertical_Corridor corridor(Vector2f(10,20), 30, 6);

    CHECK_EQUAL(6, corridor.getId());
    CHECK_EQUAL(30, corridor.getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor.getType());
    Vector3f startPoint = corridor.getStartPoint(true);
    Vector3f endPoint = corridor.getEndPoint();
    CHECK_EQUAL(10, startPoint.x);
    CHECK_EQUAL(20, startPoint.y);
    CHECK_EQUAL(0, startPoint.z);
    CHECK_EQUAL(10, endPoint.x);
    CHECK_EQUAL(20, endPoint.y);
    CHECK_EQUAL(30, endPoint.z);
}
