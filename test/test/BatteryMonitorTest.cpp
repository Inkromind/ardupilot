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

ReactiveFacadeMock* facadeMock = 0;
BattMonitorMock* batteryMock = 0;
CorridorManagerMock* corridorManagerMock = 0;

TEST_GROUP(BatteryMonitor)
{
    void setup()
    {
        facadeMock = new ReactiveFacadeMock();
        ReactiveFacadeMock::setFacade(facadeMock);
        batteryMock = new BattMonitorMock();
        corridorManagerMock = new CorridorManagerMock();
        CorridorManagerMock::setInstance(corridorManagerMock);
    }
    void teardown()
    {
        ReactiveFacadeMock::deleteFacade();
        delete facadeMock;
        delete batteryMock;
        CorridorManagerMock::deleteInstance();
        delete corridorManagerMock;
    }
};

TEST(BatteryMonitor, IsActiveTrue)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
}

TEST(BatteryMonitor, IsActiveFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(!monitor.isActive());
}

TEST(BatteryMonitor, IsActiveTrueChangedLevel)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    CHECK_TRUE(monitor.isActive());
}

TEST(BatteryMonitor, PerformNotActivated)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(!monitor.isActive());
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(false);
    mock().expectOneCall("RFland").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsTrue)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(false);
    Vector3f location = Vector3f(1,2,3);
    mock().expectOneCall("RFland").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFgetRealPosition").onObject(facadeMock).andReturnValue(&location);
    mock().expectOneCall("CMmarkCorridorsReserved").onObject(corridorManagerMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
    mock().expectOneCall("CMfreeCorridors").onObject(corridorManagerMock);
}

TEST(BatteryMonitor, PerformLandAndDisarmed)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(monitor.perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsFalse)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFdisarmMotors").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsTrue)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFdisarmMotors").onObject(facadeMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
}

TEST(BatteryMonitor, PerformReserveAndFreeCorridor)
{
    mock().expectOneCall("RFgetBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("BMbatteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(false);
    Vector3f location = Vector3f(1,2,3);
    mock().expectOneCall("RFgetRealPosition").onObject(facadeMock).andReturnValue(&location);
    mock().expectOneCall("CMmarkCorridorsReserved").onObject(corridorManagerMock).andReturnValue(true);
    mock().expectOneCall("RFland").onObject(facadeMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
    CHECK_EQUAL(1, corridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = corridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, corridors->size());
    AMW_Corridor* corridor = corridors->front();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor->getType());
    CHECK_EQUAL(location.x, corridor->getStartPoint().x);
    CHECK_EQUAL(location.y, corridor->getStartPoint().y);
    CHECK_EQUAL(location.z, corridor->getAltitude());
    uint8_t corridorId = corridor->getId();
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(false);
    mock().expectOneCall("RFland").onObject(facadeMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("CMfreeCorridors").onObject(corridorManagerMock);
    mock().expectOneCall("RFareMotorsArmed").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(monitor.perform());
    CHECK_EQUAL(1, corridorManagerMock->freeCorridorsLists.size())
    std::list<AMW_Corridor*>* freedCorridors = corridorManagerMock->freeCorridorsLists.front();
    CHECK_EQUAL(1, freedCorridors->size());
    AMW_Corridor* freedCorridor = freedCorridors->front();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) freedCorridor->getType());
    CHECK_EQUAL(location.x, freedCorridor->getStartPoint().x);
    CHECK_EQUAL(location.y, freedCorridor->getStartPoint().y);
    CHECK_EQUAL(location.z, freedCorridor->getAltitude());
    CHECK_EQUAL(corridorId, freedCorridor->getId());
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("RFisLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("RFareMotorsArmed").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(monitor.perform());
}
