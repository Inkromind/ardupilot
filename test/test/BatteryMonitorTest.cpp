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
#include  "../mocks/TaskPlannerMock.h"

//Includes
#include <AC_ReactiveFacade.h>
#include <AMW_Facade.h>
#include "../../libraries/AP_Math/vector3.h"

ReactiveFacadeMock* BMfacadeMock = 0;
BattMonitorMock* BMbatteryMock = 0;
CorridorManagerMock* BMcorridorManagerMock = 0;
AC_Behaviour_Battery_Monitor* BMmonitor = 0;
TaskPlannerMock* taskPlannerMock = 0;

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
        taskPlannerMock = new TaskPlannerMock();
        TaskPlannerMock::setPlanner(taskPlannerMock);
    }
    void teardown()
    {
        delete BMmonitor;
        ReactiveFacadeMock::deleteFacade();
        delete BMfacadeMock;
        delete BMbatteryMock;
        CorridorManagerMock::deleteInstance();
        delete BMcorridorManagerMock;
        TaskPlannerMock::deletePlanner();
        delete taskPlannerMock;
    }
};

TEST(BatteryMonitor, IsActiveTrue)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);

    CHECK_TRUE(BMmonitor->isActive());
}

TEST(BatteryMonitor, IsActiveFalse)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT);

    CHECK_FALSE(BMmonitor->isActive());
}

TEST(BatteryMonitor, IsActiveTrueChangedLevel)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->isActive());
}

TEST(BatteryMonitor, PerformNotActivated)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT);

    CHECK_FALSE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsFalse)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock("ReactiveFacade").expectOneCall("land").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotLandedCommandReturnsTrue)
{
    Vector3f location = Vector3f(1,2,3);

    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock("ReactiveFacade").expectOneCall("land").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("getRealPosition").onObject(BMfacadeMock).andReturnValue(&location);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(BMcorridorManagerMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());

    mock("CorManager").expectOneCall("freeCorridors").onObject(BMcorridorManagerMock);
}

TEST(BatteryMonitor, PerformLandAndDisarmed)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("areMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsFalse)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("areMotorsArmed").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("disarmMotors").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_FALSE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformNotDisarmedCommandReturnsTrue)
{
    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("areMotorsArmed").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("disarmMotors").onObject(BMfacadeMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->isActive());
    CHECK_TRUE(BMmonitor->perform());
}

TEST(BatteryMonitor, PerformReserveAndFreeCorridor)
{
    Vector3f location = Vector3f(1,2,3);

    mock("ReactiveFacade").expectOneCall("getBattery").onObject(BMfacadeMock).andReturnValue(BMbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AC_BATTERY_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(taskPlannerMock);
    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock("ReactiveFacade").expectOneCall("getRealPosition").onObject(BMfacadeMock).andReturnValue(&location);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(BMcorridorManagerMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("land").onObject(BMfacadeMock).andReturnValue(true);

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

    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(false);
    mock("ReactiveFacade").expectOneCall("land").onObject(BMfacadeMock).andReturnValue(true);

    CHECK_TRUE(BMmonitor->perform());

    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(BMcorridorManagerMock);
    mock("ReactiveFacade").expectOneCall("areMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

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

    mock("ReactiveFacade").expectOneCall("isLanded").onObject(BMfacadeMock).andReturnValue(true);
    mock("ReactiveFacade").expectOneCall("areMotorsArmed").onObject(BMfacadeMock).andReturnValue(false);

    CHECK_TRUE(BMmonitor->perform());
}
