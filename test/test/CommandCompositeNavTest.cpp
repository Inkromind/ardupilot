/*
 * CommandCompositeNavTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Planner/AMW_Command_Composite_Nav_Assigned_Altitude.h"

//Mocks
#include "../mocks/CommandMock.h"
#include  "../mocks/CorridorManagerMock.h"
#include  "../mocks/CorridorMock.h"
#include  "../mocks/FacadeMock.h"
#include  "../mocks/BatteryMonitorMock.h"
#include  "../mocks/TaskPlannerMock.h"

CorridorManagerMock* CCNcorridorManagerMock = 0;
FacadeMock* CCNfacadeMock = 0;
BattMonitorMock* CCNbatteryMock = 0;
TaskPlannerMock* CCNtaskPlannerMock = 0;

class DummyCompositeNavCommand : public AMW_Command_Composite_Nav_Assigned_Altitude {
public:
    DummyCompositeNavCommand(void) : AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f(0, 0)) {}
    DummyCompositeNavCommand(Vector2f newDestination) : AMW_Command_Composite_Nav_Assigned_Altitude(newDestination) {}

    AMW_Queue<AMW_Command*>* getSubCommands() {
        return &subCommands;
    }
    AMW_List<AMW_Corridor*>* getCorridors() {
        return &corridors;
    }
    void runCompletedSubCommand() {
        completedSubCommand();
    }

    void runUpdateStatus(void) {
        updateStatus();
    }

    void runStartCommand(bool attempt) {
        startCommand(attempt);
    }

    void setCommandStarted(bool started) {
        commandStarted = started;
    }
    bool getCommandStarted() {
        return commandStarted;
    }
    void setCompleted(bool newCompleted) {
        completed = newCompleted;
    }
    void setFailed(bool newFailed) {
        failed = newFailed;
    }
    bool getPaused() {
        return paused;
    }
    void setPaused(bool newPaused) {
        paused = newPaused;
    }

    Vector3f getDestination() {
        return destination;
    }

    Vector2f getStartLocation() {
        return startLocation;
    }

    CommandState getState() {
        return currentState;
    }

    void setState(CommandState state) {
        currentState = state;
    }

};

DummyCompositeNavCommand* CCNcommand = 0;

TEST_GROUP(CommandCompositeNav)
{
    void setup()
    {
        CCNcommand = new DummyCompositeNavCommand();
        CCNcorridorManagerMock = new CorridorManagerMock();
        CorridorManagerMock::setInstance(CCNcorridorManagerMock);
        CCNfacadeMock = new FacadeMock();
        FacadeMock::setFacade(CCNfacadeMock);
        CCNbatteryMock = new BattMonitorMock();
        CCNtaskPlannerMock = new TaskPlannerMock();
        TaskPlannerMock::setPlanner(CCNtaskPlannerMock);
    }
    void teardown()
    {
        delete CCNcommand;
        CorridorManagerMock::deleteInstance();
        delete CCNcorridorManagerMock;
        FacadeMock::deleteFacade();
        delete CCNfacadeMock;
        delete CCNbatteryMock;
        TaskPlannerMock::deletePlanner();
        delete CCNtaskPlannerMock;
    }
};

TEST(CommandCompositeNav, destructor) {
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand();
    CorridorMock* corridor = new CorridorMock();
    command->getCorridors()->push_back(corridor);

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;

    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->freeCorridorsLists.size())
    std::list<AMW_Corridor*>* freedCorridors = CCNcorridorManagerMock->freeCorridorsLists.front();
    CHECK_EQUAL(1, (int) freedCorridors->size());
    CHECK_EQUAL(corridor, freedCorridors->front());
}

TEST(CommandCompositeNav, startCommandBatteryTooLow) {
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT - 1);
    mock("TaskPlanner").expectOneCall("markBatteryEmpty").onObject(CCNtaskPlannerMock);

    CCNcommand->runStartCommand(true);

    CHECK_TRUE(CCNcommand->hasFailed());
}

TEST(CommandCompositeNav, startCommandAttemptTrueReserving) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));

    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->runStartCommand(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS,
            command->getState());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL(3, command->getCorridors()->size())
    CHECK_EQUAL(corridor1, command->getCorridors()->front());
    CHECK_EQUAL(corridor2, command->getCorridors()->get(1));
    CHECK_EQUAL(corridor3, command->getCorridors()->back());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}

TEST(CommandCompositeNav, startCommandAttemptFalseReserving) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));

    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", AMW_CORRIDOR_MAX_FAILURES).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->runStartCommand(false);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS,
            command->getState());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL(3, command->getCorridors()->size())
    CHECK_EQUAL(corridor1, command->getCorridors()->front());
    CHECK_EQUAL(corridor2, command->getCorridors()->get(1));
    CHECK_EQUAL(corridor3, command->getCorridors()->back());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}

TEST(CommandCompositeNav, startCommandAttemptTrueNotReserving) {
    Vector3f position = Vector3f(1, 2, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));

    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(false);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->runStartCommand(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::FAILED,
            command->getState());
    CHECK_TRUE(command->hasFailed());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}
TEST(CommandCompositeNav, startCommandAttemptFalseNotReserving) {
    Vector3f position = Vector3f(1, 2, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));

    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 5).andReturnValue(false);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->runStartCommand(false);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::FAILED,
            command->getState());
    CHECK_TRUE(command->hasFailed());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}
TEST(CommandCompositeNav, startCommandAlreadyStarted) {
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    CCNcommand->runStartCommand(true);

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
}

TEST(CommandCompositeNav, updateStatusCompleted) {
    CCNcommand->setCompleted(true);

    CCNcommand->runUpdateStatus();

    CHECK_TRUE(CCNcommand->isComplete());
}
TEST(CommandCompositeNav, updateStatusFailed) {
    CCNcommand->setFailed(true);

    CCNcommand->runUpdateStatus();

    CHECK_TRUE(CCNcommand->hasFailed());
}
TEST(CommandCompositeNav, updateStatusSubCommandsEmpty) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();

    CCNcommand->runUpdateStatus();

    CHECK_TRUE(CCNcommand->isComplete());
}
TEST(CommandCompositeNav, updateStatusInitReachedDestination) {
    CCNcommand->getCorridors()->push_back(new CorridorMock());
    mock("Facade").expectOneCall("destinationReached").withDoubleParameter("destinationX", 0).
            withDoubleParameter("destinationY", 0).withDoubleParameter("destinationZ", 0)
            .onObject(CCNfacadeMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_TRUE(CCNcommand->isComplete());
}
TEST(CommandCompositeNav, updateStatusInitConflict)
{
    mock("Facade").expectOneCall("destinationReached").withDoubleParameter("destinationX", 0).
            withDoubleParameter("destinationY", 0).withDoubleParameter("destinationZ", 0)
            .onObject(CCNfacadeMock).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);

    CCNcommand->runUpdateStatus();

    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::INIT,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, updateStatusCorridorConflictNormal3CommandsRemaining) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor1).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_EQUAL(2, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(corridor1, CCNcommand->getCorridors()->front());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusCorridorConflictNormal2CommandsRemaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor1).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_EQUAL(3, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(corridor1, CCNcommand->getCorridors()->front());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusCorridorConflictNormal1CommandsRemaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor1).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_EQUAL(4, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(corridor1, CCNcommand->getCorridors()->front());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusCorridorConflictNormal0CommandsRemaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor1).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setReverseDirection").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_EQUAL(4, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(corridor1, CCNcommand->getCorridors()->front());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}

TEST(CommandCompositeNav, updateStatusCorridorConflictReturnToStart) {
    Vector3f position = Vector3f(1, 2, 10);
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    CHECK_FALSE(CCNcommand->isComplete());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
    CHECK_EQUAL(2, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(1, CCNcommand->getCorridors()->size());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = CCNcorridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    AMW_Corridor* corridor = corridors->front();
    CHECK_EQUAL(corridor, CCNcommand->getCorridors()->front());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor->getType());
    CHECK_EQUAL(position.x, corridor->getStartPoint().x);
    CHECK_EQUAL(position.y, corridor->getStartPoint().y);
    CHECK_EQUAL(position.z, corridor->getAltitude());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusCorridorConflictLand) {
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);

    CCNcommand->runUpdateStatus();

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, updateStatusWaitingCorridorsReserved) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL,
            CCNcommand->getState());
    CHECK_TRUE(CCNcommand->hasStarted());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}

TEST(CommandCompositeNav, updateStatusWaitingReservationFailed) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::FAILED,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_TRUE(CCNcommand->hasFailed());
}
TEST(CommandCompositeNav, updateStatusWaitingReserving) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("isReservingCorridors").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusWaitingNotReservingFailed) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("isReservingCorridors").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 5).andReturnValue(false);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::FAILED,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_TRUE(CCNcommand->hasFailed());
}
TEST(CommandCompositeNav, updateStatusWaitingNotReserving) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("isReservingCorridors").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 5).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::WAITING_FOR_CORRIDORS,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());


    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusNormalNotReserved) {
    Vector3f position = Vector3f(1, 2, 10);
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
    CHECK_EQUAL(2, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(1, CCNcommand->getCorridors()->size());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = CCNcorridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    AMW_Corridor* corridor = corridors->front();
    CHECK_EQUAL(corridor, CCNcommand->getCorridors()->front());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor->getType());
    CHECK_EQUAL(position.x, corridor->getStartPoint().x);
    CHECK_EQUAL(position.y, corridor->getStartPoint().y);
    CHECK_EQUAL(position.z, corridor->getAltitude());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusNormalReserved) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusReturnToStartNotReservedNoCorridors) {
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);

    CCNcommand->runUpdateStatus();

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());
}
TEST(CommandCompositeNav, updateStatusReturnToStartNotReservedCorridors) {
    Vector3f position = Vector3f(1, 2, 10);
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
    CHECK_EQUAL(2, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(1, CCNcommand->getCorridors()->size());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = CCNcorridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    AMW_Corridor* corridor = corridors->front();
    CHECK_EQUAL(corridor, CCNcommand->getCorridors()->front());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor->getType());
    CHECK_EQUAL(position.x, corridor->getStartPoint().x);
    CHECK_EQUAL(position.y, corridor->getStartPoint().y);
    CHECK_EQUAL(position.z, corridor->getAltitude());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusReturnToStartReservedCorridors) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusLandNotReservedNoCorridors) {
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);

    CCNcommand->runUpdateStatus();

    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_FALSE(CCNcommand->hasFailed());
}
TEST(CommandCompositeNav, updateStatusLandNotReservedCorridors) {
    CorridorMock* corridor1 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
    CHECK_EQUAL(1, CCNcommand->getCorridors()->size());
    CHECK_EQUAL(corridor1, CCNcommand->getCorridors()->front());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, updateStatusLandReservedCorridors) {
    CorridorMock* corridor1 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);

    CCNcommand->runUpdateStatus();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, completedSubCommandNormal2Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();;
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("Corridor").expectOneCall("setCompleted").onObject(corridor1).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", true);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL,
            CCNcommand->getState());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, completedSubCommandNormal1Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", true);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL,
            CCNcommand->getState());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, completedSubCommandNormal0Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL);

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::NORMAL,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, completedSubCommandRTS3Remaining) {
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("Corridor").expectOneCall("setCompleted").onObject(corridor3).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor3).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", true);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, completedSubCommandRTS2Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("Corridor").expectOneCall("setCompleted").onObject(corridor2).withParameter("newValue", true);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor2).withParameter("newValue", false);
    mock("Corridor").expectOneCall("setInCorridor").onObject(corridor1).withParameter("newValue", true);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
}
TEST(CommandCompositeNav, completedSubCommandRTS1Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CorridorMock* corridor2 = new CorridorMock();
    CorridorMock* corridor3 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->getCorridors()->push_back(corridor2);
    CCNcommand->getCorridors()->push_back(corridor3);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, completedSubCommandRTS0Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::RETURNTOSTART);
    CCNcommand->setCommandStarted(true);

    CCNcommand->runCompletedSubCommand();

    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_EQUAL(3, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::INIT,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, completedSubCommandLand1Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CorridorMock* corridor1 = new CorridorMock();
    CCNcommand->getCorridors()->push_back(corridor1);
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    CCNcommand->runCompletedSubCommand();

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, completedSubCommandLand0Remaining) {
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    delete CCNcommand->getSubCommands()->front();
    CCNcommand->getSubCommands()->pop();
    CCNcommand->setState(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::LAND);
    CCNcommand->setCommandStarted(true);

    CCNcommand->runCompletedSubCommand();

    CHECK_FALSE(CCNcommand->hasStarted());
    CHECK_EQUAL(3, CCNcommand->getSubCommands()->size());
    CHECK_EQUAL(AMW_Command_Composite_Nav_Assigned_Altitude::CommandState::INIT,
            CCNcommand->getState());
}
TEST(CommandCompositeNav, scenarioNormal) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("isReservingCorridors").onObject(CCNcorridorManagerMock).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("takeoff").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("navigateTo").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_TRUE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(false);
    mock("Facade").expectOneCall("land").onObject(CCNfacadeMock).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_TRUE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_TRUE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    delete command;
}
TEST(CommandCompositeNav, scenarioReservationFailure) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("reservationHasFailed").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_TRUE(command->hasFailed());

    delete command;
}
TEST(CommandCompositeNav, scenarioCorridorConflict) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("takeoff").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("navigateTo").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_TRUE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", position.x).
            withDoubleParameter("destinationY", position.y).withDoubleParameter("destinationZ", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("navigateTo").onObject(CCNfacadeMock).withDoubleParameter("destinationX", position.x).
            withDoubleParameter("destinationY", position.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_TRUE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());
    CHECK_TRUE(corridor1->isReverseDirection());
    CHECK_TRUE(corridor2->isReverseDirection());
    CHECK_TRUE(corridor3->isReverseDirection());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", position.x).
            withDoubleParameter("destinationY", position.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(false);
    mock("Facade").expectOneCall("land").onObject(CCNfacadeMock).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());
    CHECK_TRUE(corridor1->isReverseDirection());
    CHECK_TRUE(corridor2->isReverseDirection());
    CHECK_TRUE(corridor3->isReverseDirection());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("loiter").onObject(CCNfacadeMock).andReturnValue(true);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CCNfacadeMock).andReturnValue(10);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());

    Vector3f position2 = Vector3f(5, 6, 0);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CCNfacadeMock).andReturnValue(50);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position2);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_EQUAL(2, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.back();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    corridor2 = reservedCorridors->front();
    corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position2).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position2).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(5, 6)).length());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(2000.0);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 2000).andReturnValue(false);
    mock("Facade").expectOneCall("takeoff").onObject(CCNfacadeMock).withDoubleParameter("altitude", 2000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());
    CHECK_FALSE(corridor1->isReverseDirection());
    CHECK_FALSE(corridor2->isReverseDirection());
    CHECK_FALSE(corridor3->isReverseDirection());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}
TEST(CommandCompositeNav, scenarioDoubleCorridorConflict) {
    Vector3f position = Vector3f(1, 2, 0);
    Vector3f destination = Vector3f(10, 20, 0);
    DummyCompositeNavCommand* command = new DummyCompositeNavCommand(Vector2f(10, 20));
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    std::list<AMW_Corridor*>* reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.front();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    AMW_Corridor* corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    AMW_Corridor* corridor2 = reservedCorridors->front();
    AMW_Corridor* corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    Vector3f point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(1, 2)).length());
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("takeoff").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 1000).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("navigateTo").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_TRUE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(1000.0);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", position.x).
            withDoubleParameter("destinationY", position.y).withDoubleParameter("destinationZ", 1000).andReturnValue(false);
    mock("Facade").expectOneCall("navigateTo").onObject(CCNfacadeMock).withDoubleParameter("destinationX", position.x).
            withDoubleParameter("destinationY", position.y).withDoubleParameter("destinationZ", 1000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_FALSE(corridor1->isInCorridor());
    CHECK_TRUE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());
    CHECK_TRUE(corridor1->isReverseDirection());
    CHECK_TRUE(corridor2->isReverseDirection());
    CHECK_TRUE(corridor3->isReverseDirection());

    Vector3f position2 = Vector3f(7, 8, 0);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("markCorridorConflictResolved").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position2);
    mock("CorManager").expectOneCall("markCorridorsReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(false);
    mock("Facade").expectOneCall("land").onObject(CCNfacadeMock).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_EQUAL(1, (int) CCNcorridorManagerMock->markCorridorsReservedLists.size())
    std::list<AMW_Corridor*>* corridors = CCNcorridorManagerMock->markCorridorsReservedLists.front();
    CHECK_EQUAL(1, (int) corridors->size());
    AMW_Corridor* landCorridor = corridors->front();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) landCorridor->getType());
    CHECK_EQUAL(position2.x, landCorridor->getStartPoint().x);
    CHECK_EQUAL(position2.y, landCorridor->getStartPoint().y);
    CHECK_EQUAL(position2.z, landCorridor->getAltitude());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("Facade").expectOneCall("isLanded").onObject(CCNfacadeMock).andReturnValue(true);
    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("loiter").onObject(CCNfacadeMock).andReturnValue(true);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CCNfacadeMock).andReturnValue(10);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());

    position2 = Vector3f(5, 6, 0);

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getTimeMillis").onObject(CCNfacadeMock).andReturnValue(50);
    mock("Facade").expectOneCall("destinationReached").onObject(CCNfacadeMock).withDoubleParameter("destinationX", destination.x).
            withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", 0).andReturnValue(false);
    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("Facade").expectOneCall("getBattery").onObject(CCNfacadeMock).andReturnValue(CCNbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(AMW_COMMAND_BATTERY_TAKEOFF_LIMIT);
    mock("CorManager").expectOneCall("reserveCorridors").onObject(CCNcorridorManagerMock).withParameter("maxFailures", 1).andReturnValue(true);
    mock("Facade").expectOneCall("getRealPosition").onObject(CCNfacadeMock).andReturnValue(&position2);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->hasStarted());
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_EQUAL(2, (int) CCNcorridorManagerMock->reserveCorridorsLists.size());
    reservedCorridors = CCNcorridorManagerMock->reserveCorridorsLists.back();
    CHECK_EQUAL(3, (int) reservedCorridors->size());
    corridor1 = reservedCorridors->front();
    reservedCorridors->pop_front();
    corridor2 = reservedCorridors->front();
    corridor3 = reservedCorridors->back();
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor1->getType());
    point = corridor1->getStartPoint(true);
    CHECK_EQUAL(0, (point - position2).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::HORIZONTAL, (uint8_t) corridor2->getType());
    point = corridor2->getStartPoint(true);
    CHECK_EQUAL(0, (point - position2).length());
    CHECK_EQUAL(0, corridor1->getAltitude());
    point = corridor2->getEndPoint();
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor2->getAltitude());
    CHECK_EQUAL((uint8_t) AMW_Corridor::Type::VERTICAL, (uint8_t) corridor3->getType());
    point = corridor3->getStartPoint(true);
    CHECK_EQUAL(0, (point - destination).length());
    CHECK_EQUAL(0, corridor3->getAltitude());
    CHECK_EQUAL(0, (command->getStartLocation() - Vector2f(5, 6)).length());

    mock("CorManager").expectOneCall("hasCorridorConflict").onObject(CCNcorridorManagerMock).andReturnValue(false);
    mock("CorManager").expectOneCall("corridorsAreReserved").onObject(CCNcorridorManagerMock).andReturnValue(true);
    mock("CorManager").expectOneCall("getReservedAltitude").onObject(CCNcorridorManagerMock).andReturnValue(2000.0);
    mock("Facade").expectOneCall("altitudeReached").onObject(CCNfacadeMock).withDoubleParameter("altitude", 2000).andReturnValue(false);
    mock("Facade").expectOneCall("takeoff").onObject(CCNfacadeMock).withDoubleParameter("altitude", 2000).andReturnValue(true);

    command->run(true);

    mock("CorManager").checkExpectations();
    mock("CorManager").clear();
    CHECK_FALSE(command->isComplete());
    CHECK_FALSE(command->hasFailed());
    CHECK_TRUE(command->hasStarted());
    CHECK_TRUE(corridor1->isInCorridor());
    CHECK_FALSE(corridor2->isInCorridor());
    CHECK_FALSE(corridor3->isInCorridor());
    CHECK_FALSE(corridor1->isReverseDirection());
    CHECK_FALSE(corridor2->isReverseDirection());
    CHECK_FALSE(corridor3->isReverseDirection());

    mock("CorManager").expectOneCall("freeCorridors").onObject(CCNcorridorManagerMock);

    delete command;
}


