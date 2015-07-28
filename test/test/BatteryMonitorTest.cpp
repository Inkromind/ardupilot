/*
 * BatteryMonitorTest.cpp
 *
 *  Created on: 27-jul.-2015
 *      Author: Arne
 */


#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "../../libraries/AC_SafetyControl/AC_Behaviour_Battery_Monitor.h"
#include <AC_ReactiveFacade.h>
#include <AP_BattMonitor.h>
#include "../../libraries/AP_Math/vector3.h"

#include  "../mocks/CorridorManagerMock.h"
#include  "../mocks/BatteryMonitorMock.h"
#include  "../mocks/ReactiveFacadeMock.h"

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
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
}

TEST(BatteryMonitor, IsActiveFalse)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(!monitor.isActive());
}

TEST(BatteryMonitor, IsActiveTrueChangedLevel)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    CHECK_TRUE(monitor.isActive());
}

TEST(BatteryMonitor, PerformNotActivated)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(!monitor.isActive());
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsFalse)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(false);
    mock().expectOneCall("land").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsTrue)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(false);
    Vector3f location = Vector3f(1,2,3);
    mock().expectOneCall("land").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("getRealPosition").onObject(facadeMock).andReturnValue(&location);
    mock().expectOneCall("markCorridorsReserved").onObject(corridorManagerMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
    mock().expectOneCall("freeCorridors").onObject(corridorManagerMock);
}

TEST(BatteryMonitor, PerformLandAndDisarmed)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("areMotorsArmed").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(monitor.perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsFalse)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("areMotorsArmed").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("disarmMotors").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(!monitor.perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsTrue)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("areMotorsArmed").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("disarmMotors").onObject(facadeMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
}

TEST(BatteryMonitor, PerformReserveAndFreeCorridor)
{
    mock().expectOneCall("getBattery").onObject(facadeMock).andReturnValue(batteryMock);
    mock().expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    AC_Behaviour_Battery_Monitor monitor;
    CHECK_TRUE(monitor.isActive());
    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(false);
    Vector3f location = Vector3f(1,2,3);
    mock().expectOneCall("getRealPosition").onObject(facadeMock).andReturnValue(&location);
    mock().expectOneCall("markCorridorsReserved").onObject(corridorManagerMock).andReturnValue(true);
    mock().expectOneCall("land").onObject(facadeMock).andReturnValue(true);
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

    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(false);
    mock().expectOneCall("land").onObject(facadeMock).andReturnValue(true);
    CHECK_TRUE(monitor.perform());
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("freeCorridors").onObject(corridorManagerMock);
    mock().expectOneCall("areMotorsArmed").onObject(facadeMock).andReturnValue(false);
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

    mock().expectOneCall("isLanded").onObject(facadeMock).andReturnValue(true);
    mock().expectOneCall("areMotorsArmed").onObject(facadeMock).andReturnValue(false);
    CHECK_TRUE(monitor.perform());
}
