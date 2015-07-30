/*
 * BatteryMonitorTest.cpp
 *
 *  Created on: 27-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AC_SafetyControl/AC_Behaviour_Battery_Monitor.h"

//Mocks
#include  "../mocks/CorridorManagerMock.h"
#include  "../mocks/BatteryMonitorMock.h"
#include  "../mocks/ReactiveFacadeMock.h"

//Includes
#include <AC_ReactiveFacade.h>
#include <AP_BattMonitor.h>
#include <AMW_Facade.h>
#include "../../libraries/AP_Math/vector3.h"

ReactiveFacadeMock* BMfacadeMock = 0;
BattMonitorMock* BMbatteryMock = 0;
CorridorManagerMock* BMcorridorManagerMock = 0;
AC_Behaviour_Battery_Monitor* BMmonitor = 0;

TEST_GROUP(BatteryMonitor)
{
    void setup()
    {
        BMfacadeMock = new ReactiveFacadeMock();
        ReactiveFacadeMock::setFacade(BMfacadeMock);
        BMbatteryMock = new BattMonitorMock();
        BMcorridorManagerMock = new CorridorManagerMock();
        CorridorManagerMock::setInstance(BMcorridorManagerMock);
        BMmonitor = new AC_Behaviour_Battery_Monitor();
    }
    void teardown()
    {
        delete BMmonitor;
        ReactiveFacadeMock::deleteFacade();
        delete BMfacadeMock;
        delete BMbatteryMock;
        CorridorManagerMock::deleteInstance();
        delete BMcorridorManagerMock;
    }
};

TEST(BatteryMonitor, IsActiveTrue)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);

    CHECK_TRUE(BMmonitor->isActive());
}

TEST(BatteryMonitor, IsActiveFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT);

    CHECK_FALSE(BMmonitor->isActive());
}

TEST(BatteryMonitor, IsActiveTrueChangedLevel)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->isActive());
}

TEST(BatteryMonitor, PerformNotActivated)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT);

    CHECK_FALSE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock().expectOneCall("RFland").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsTrue)
{
    Vector3f location = Vector3f(1,2,3);

    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock().expectOneCall("RFland").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFgetRealPosition").onObject(BMfacadeMock).andReturnValue(&location);
    mock().expectOneCall("CMmarkCorridorsReserved").onObject(BMcorridorManagerMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());

    mock().expectOneCall("CMfreeCorridors").onObject(BMcorridorManagerMock);
}

TEST(BatteryMonitor, PerformLandAndDisarmed)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFdisarmMotors").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsTrue)
{
    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFdisarmMotors").onObject(BMfacadeMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformReserveAndFreeCorridor)
{
    Vector3f location = Vector3f(1,2,3);

    mock().expectOneCall("RFgetBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock().expectOneCall("RFgetRealPosition").onObject(BMfacadeMock).andReturnValue(&location);
    mock().expectOneCall("CMmarkCorridorsReserved").onObject(BMcorridorManagerMock).andReturnValue(true);
    mock().expectOneCall("RFland").onObject(BMfacadeMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());

    CHECK_EQUAL(1, (int) BMcorridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = BMcorridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    AMW_Corridor* corridor = corridors->front();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor->getType());
    CHECK_EQUAL(location.x, corridor->getStartPoint().x);
    CHECK_EQUAL(location.y, corridor->getStartPoint().y);
    CHECK_EQUAL(location.z, corridor->getAltitude());
    uint8_t corridorId = corridor->getId();

    mock().checkExpectations();

    mock().clear();
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock().expectOneCall("RFland").onObject(BMfacadeMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->perform());

    mock().checkExpectations();

    mock().clear();
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("CMfreeCorridors").onObject(BMcorridorManagerMock);
    mock().expectOneCall("RFareMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->perform());

    CHECK_EQUAL(1, (int) BMcorridorManagerMock->freeCorridorsLists.size())
    std::list<AMW_Corridor*>* freedCorridors = BMcorridorManagerMock->freeCorridorsLists.front();
    CHECK_EQUAL(1, (int) freedCorridors->size());
    AMW_Corridor* freedCorridor = freedCorridors->front();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) freedCorridor->getType());
    CHECK_EQUAL(location.x, freedCorridor->getStartPoint().x);
    CHECK_EQUAL(location.y, freedCorridor->getStartPoint().y);
    CHECK_EQUAL(location.z, freedCorridor->getAltitude());
    CHECK_EQUAL(corridorId, freedCorridor->getId());
    mock().checkExpectations();

    mock().clear();
    mock().expectOneCall("RFisLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->perform());
}
