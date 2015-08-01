/*
 * SequencerTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Planner/AMW_Sequencer.h"

//Mocks
#include "../mocks/TaskMock.h"
#include "../mocks/CommandMock.h"
#include "../mocks/TaskPlannerMock.h"
#include "../mocks/FacadeMock.h"

//Includes

class DummySequencer : public AMW_Sequencer {
public:
    bool isPaused() {
        return paused;
    }
    void setPaused(bool val) {
        paused = val;
    }
    AMW_Task* getCurrentTask() {
        return currentTask;
    }
    void setCurrentTask(AMW_Task* task) {
        currentTask = task;
    }
    AMW_Task* getTempTask() {
        return tempTask;
    }
    void setTempTask(AMW_Task* task) {
        tempTask = task;
    }
    AMW_Command* getCurrentPlan() {
        return currentPlan;
    }
    void setCurrentPlan(AMW_Command* plan) {
        currentPlan = plan;
    }
    AMW_Command* getTempPlan() {
        return tempPlan;
    }
    void setTempPlan(AMW_Command* plan) {
        tempPlan = plan;
    }
    bool isExecutingCurrentTask() {
        return executingCurrentTask;
    }
    void setExecutingCurrentTask(bool val) {
        executingCurrentTask = val;
    }

    void runStartNewTask(AMW_Task* newTask) {
        startNewTask(newTask);
    }

    void runTryNewTask(AMW_Task* newTask) {
        tryNewTask(newTask);
    }

    void runExecuteCurrentTask() {
        executeCurrentTask();
    }

    void runExecuteTempTask() {
        executeTempTask();
    }

    void runCheckExecutingTask() {
        checkExecutingTask();
    }

    void runCancelNewTaskContinueCurrentTask(bool removeNewTask) {
        cancelTempTaskContinueCurrentTask(removeNewTask);
    }
};

DummySequencer* Ssequencer = 0;
TaskPlannerMock* StaskPlannerMock = 0;
FacadeMock* SfacadeMock = 0;

TEST_GROUP(Sequencer)
{
    void setup()
    {
        Ssequencer = new DummySequencer();
        Ssequencer->setPaused(false);
        Ssequencer->init();
        StaskPlannerMock = new TaskPlannerMock();
        TaskPlannerMock::setPlanner(StaskPlannerMock);
        SfacadeMock = new FacadeMock();
        FacadeMock::setFacade(SfacadeMock);
    }
    void teardown()
    {
        delete Ssequencer;
        TaskPlannerMock::deletePlanner();
        delete StaskPlannerMock;
        FacadeMock::deleteFacade();
        delete SfacadeMock;
    }
};

TEST(Sequencer, tryNewTaskNullTask)
{
    TaskMock task;
    TaskMock task2;
    Ssequencer->setTempTask(&task);
    Ssequencer->setCurrentTask(&task2);
    Ssequencer->setExecutingCurrentTask(true);

    Ssequencer->runTryNewTask(0);

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, tryNewTaskSameAsTempTask)
{
    TaskMock task;
    TaskMock task2;
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempTask(&task);
    Ssequencer->setTempPlan(plan);
    Ssequencer->setCurrentTask(&task2);
    Ssequencer->setExecutingCurrentTask(true);

    Ssequencer->runTryNewTask(&task);

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, tryNewTaskExistingTempPlanNoNewPlan)
{
    TaskMock task;
    TaskMock task2;
    TaskMock* task3 = new TaskMock();
    Ssequencer->setTempTask(&task);
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runTryNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completedTaskList.front());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, tryNewTaskExistingTempPlanNewPlan)
{
    TaskMock task;
    TaskMock task2;
    TaskMock* task3 = new TaskMock();
    Ssequencer->setTempTask(&task);
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    CommandMock* plan2 = new CommandMock();

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runTryNewTask(task3);

    CHECK_EQUAL(task3, Ssequencer->getTempTask());
    CHECK_EQUAL(plan2, Ssequencer->getTempPlan());
    CHECK_FALSE(Ssequencer->isExecutingCurrentTask());

    delete task3;
}
TEST(Sequencer, tryNewTaskNewPlanCurrentPlan)
{
    TaskMock task2;
    TaskMock* task3 = new TaskMock();
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    CommandMock* plan2 = new CommandMock();

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan);

    Ssequencer->runTryNewTask(task3);

    CHECK_EQUAL(task3, Ssequencer->getTempTask());
    CHECK_EQUAL(plan2, Ssequencer->getTempPlan());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_FALSE(Ssequencer->isExecutingCurrentTask());

    delete task3;
}
TEST(Sequencer, startNewTaskNullTask)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);
    TaskMock task;
    Ssequencer->setCurrentTask(&task);

    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runStartNewTask(0);

    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
}
TEST(Sequencer, startNewTaskExistingCurrentPlanNoPlan)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task;
    Ssequencer->setCurrentTask(&task);
    TaskMock task2;
    Ssequencer->setTempTask(&task);
    TaskMock* task3 = new TaskMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runStartNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completedTaskList.front());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, startNewTaskExistingCurrentPlanPlan)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task;
    Ssequencer->setCurrentTask(&task);
    TaskMock task2;
    Ssequencer->setTempTask(&task);
    TaskMock* task3 = new TaskMock();
    Ssequencer->setExecutingCurrentTask(false);
    CommandMock* plan2 = new CommandMock();

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue(plan2);

    Ssequencer->runStartNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(task3, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
    delete task3;
}
TEST(Sequencer, startNewTaskExistingTempPlanNoPlan)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task;
    Ssequencer->setCurrentTask(&task);
    TaskMock task2;
    Ssequencer->setTempTask(&task);
    TaskMock* task3 = new TaskMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runStartNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completedTaskList.front());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, executeCurrentTaskPaused)
{
    Ssequencer->setPaused(true);

    Ssequencer->runExecuteCurrentTask();
}
TEST(Sequencer, executeCurrentTaskCompleted)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock* task = new TaskMock();
    Ssequencer->setCurrentTask(task);

    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteCurrentTask();

    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, executeCurrentTask)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock* task = new TaskMock();
    Ssequencer->setCurrentTask(task);

    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan).andReturnValue(false);

    Ssequencer->runExecuteCurrentTask();

    CHECK_EQUAL(task, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());

    delete task;
}
TEST(Sequencer, executeCurrentTaskFailed)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock* task = new TaskMock();
    Ssequencer->setCurrentTask(task);

    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteCurrentTask();

    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, executeTempTaskPaused)
{
    Ssequencer->setPaused(true);

    Ssequencer->runExecuteTempTask();
}
TEST(Sequencer, executeTempTaskCompleted)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock* task = new TaskMock();
    Ssequencer->setTempTask(task);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteTempTask();

    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, executeTempTaskFailed)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);

    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan).andReturnValue(true);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->runExecuteTempTask();

    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
}
TEST(Sequencer, executeTempTaskStarted)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasStarted").onObject(plan).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("switchedToTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteTempTask();

    CHECK_EQUAL(&task, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
    CHECK_EQUAL(1, (int) StaskPlannerMock->switchedToTaskList.size());
    CHECK_EQUAL(&task, StaskPlannerMock->switchedToTaskList.front());
}
TEST(Sequencer, pauseMissionAlreadyPaused)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setPaused(true);

    Ssequencer->pauseMission();
}
TEST(Sequencer, pauseMissionExecutingCurrentTask)
{
    CommandMock* plan = new CommandMock();
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setTempPlan(plan2);

    mock("Command").expectOneCall("pause").onObject(plan);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->pauseMission();

    CHECK_TRUE(Ssequencer->isPaused());
}
TEST(Sequencer, pauseMissionExecutingCurrentTaskNoPlan)
{
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);

    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->pauseMission();

    CHECK_TRUE(Ssequencer->isPaused());
}
TEST(Sequencer, pauseMissionNotExecutingCurrentTaskPlan)
{
    CommandMock* plan = new CommandMock();
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setTempPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("Command").expectOneCall("pause").onObject(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->pauseMission();

    CHECK_TRUE(Ssequencer->isPaused());
}
TEST(Sequencer, pauseMissionNotExecutingCurrentTaskNoPlan)
{
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setExecutingCurrentTask(false);

    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->pauseMission();

    CHECK_TRUE(Ssequencer->isPaused());
}
TEST(Sequencer, resumeMissionNotPaused)
{
    CommandMock* plan = new CommandMock();
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setTempPlan(plan2);

    Ssequencer->resumeMission();
}
TEST(Sequencer, resumeMissionForceNoFirstTask)
{
    Ssequencer->setPaused(true);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->resumeMission();

    CHECK_FALSE(Ssequencer->isPaused());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
}
TEST(Sequencer, resumeMissionForceExecutingAsTempNoPlan)
{
   Ssequencer->setPaused(true);
   TaskMock task;
   Ssequencer->setTempTask(&task);
   CommandMock* plan = new CommandMock();
   Ssequencer->setCurrentPlan(plan);
   TaskMock task2;
   Ssequencer->setCurrentTask(&task2);
   CommandMock* plan2 = new CommandMock();

   mock("TaskPlanner").setData("forceFirstTask", true);
   mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task);
   mock("Task").expectOneCall("generatePlan").onObject(&task).andReturnValue(plan2);

   Ssequencer->resumeMission();

   CHECK_EQUAL(0, Ssequencer->getTempTask());
   CHECK_EQUAL(&task, Ssequencer->getCurrentTask());
   CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
   CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceExecutingAsTempWithPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task);
    mock("Command").expectOneCall("resume").onObject(plan2);
    mock("TaskPlanner").expectOneCall("switchedToTask").onObject(StaskPlannerMock);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceExecutingAsCurrentNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceExecutingAsCurrentWithPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Command").expectOneCall("resume").onObject(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceCurrentTaskNotExecutingPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Command").expectOneCall("resume").onObject(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceCurrentTaskNotExecutingNoPlanPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionForceCurrentTaskNotExecutingNoPlanNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    TaskMock* task2 = new TaskMock();
    Ssequencer->setCurrentTask(task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task2, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, resumeMissionForceNewTaskPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);
    TaskMock task3;
    CommandMock* plan3 = new CommandMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task3);
    mock("Task").expectOneCall("generatePlan").onObject(&task3).andReturnValue(plan3);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task3, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan3, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, resumeMissionForceNewTaskNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setTempPlan(plan2);
    TaskMock* task3 = new TaskMock();

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task3);
    mock("Task").expectOneCall("generatePlan").onObject(task3).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, resumeMissionNoFirstTaskExecutingCurrentNoTask)
{
    Ssequencer->setPaused(true);
    Ssequencer->setExecutingCurrentTask(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoFirstTaskExecutingCurrentTaskNoPlanNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock* task2 = new TaskMock();
    Ssequencer->setCurrentTask(task2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task2, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, resumeMissionNoFirstTaskExecutingCurrentTaskNoPlanPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoFirstTaskExecutingCurrentTaskPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoFirstTaskNotExecutingCurrentTaskNoTask)
{
    Ssequencer->setPaused(true);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoFirstTaskNotExecutingCurrentTaskNoPlanNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock* task = new TaskMock();
    Ssequencer->setTempTask(task);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Task").expectOneCall("generatePlan").onObject(task).andReturnValue((AMW_Command*) 0);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, resumeMissionNoFirstTaskNotExecutingCurrentTaskNoPlanPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Task").expectOneCall("generatePlan").onObject(&task).andReturnValue(plan);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoFirstTaskNotExecutingCurrentTaskPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Command").expectOneCall("resume").onObject(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionExecutingAsTempTaskPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task);
    mock("Command").expectOneCall("resume").onObject(plan);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionExecutingAsTempTaskNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task);
    mock("Task").expectOneCall("generatePlan").onObject(&task).andReturnValue(plan);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionExecutingAsCurrentTaskPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionExecutingAsCurrentTaskNoPlan)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionCurrentTaskNewTask)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task3;
    CommandMock* plan3 = new CommandMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task3);
    mock("Task").expectOneCall("generatePlan").onObject(&task3).andReturnValue(plan3);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(&task3, Ssequencer->getTempTask());
    CHECK_EQUAL(plan3, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, resumeMissionNoCurrentTaskNewTask)
{
    Ssequencer->setPaused(true);
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan2);

    Ssequencer->resumeMission();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTaskNoCurrentPlan)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTaskNoTempPlan)
{
    TaskMock task;
    Ssequencer->setCurrentTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setCurrentPlan(plan);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskNoCurrentTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Task").expectOneCall("generatePlan").onObject(&task2).andReturnValue(plan2);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskForceTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task3;
    CommandMock* plan3 = new CommandMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task3);
    mock("Task").expectOneCall("generatePlan").onObject(&task3).andReturnValue(plan3);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task3, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan3, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskDifferentFromCurrentDifferentFromTemp)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    TaskMock task3;
    CommandMock* plan3 = new CommandMock();
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task3);
    mock("Task").expectOneCall("generatePlan").onObject(&task3).andReturnValue(plan3);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan2);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(&task3, Ssequencer->getTempTask());
    CHECK_EQUAL(plan3, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_FALSE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskDifferentFromCurrentSameAsTemp)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
    CHECK_EQUAL(plan, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskSameAsCurrentTempTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskSameAsCurrentNotExecutingCurrent)
{
    Ssequencer->setExecutingCurrentTask(false);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);
    mock("Command").expectOneCall("resume").onObject(plan2);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, checkExecutingTaskSameAsCurrentExecutingCurrent)
{
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(&task2);

    Ssequencer->runCheckExecutingTask();

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
}
TEST(Sequencer, runNoCurrentPlanExecutingCurrentTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);

    Ssequencer->run();
}
TEST(Sequencer, runCurrentPlanExecutingCurrentTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);

    Ssequencer->run();
}
TEST(Sequencer, runNoTempPlanNotExecutingCurrentTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);

    Ssequencer->run();
}
TEST(Sequencer, runTempPlanNotExecutingCurrentTask)
{
    TaskMock task;
    Ssequencer->setTempTask(&task);
    CommandMock* plan = new CommandMock();
    Ssequencer->setTempPlan(plan);
    TaskMock task2;
    Ssequencer->setCurrentTask(&task2);
    CommandMock* plan2 = new CommandMock();
    Ssequencer->setCurrentPlan(plan2);
    Ssequencer->setExecutingCurrentTask(false);

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);
    mock("Command").expectOneCall("run").onObject(plan).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan).andReturnValue(false);
    mock("Command").expectOneCall("hasStarted").onObject(plan).andReturnValue(false);

    Ssequencer->run();
}
TEST(Sequencer, scenarioNormal)
{
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);

    Ssequencer->run();

    // First Task
    TaskMock* task1 = new TaskMock();
    CommandMock* plan1 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Task").expectOneCall("generatePlan").onObject(task1).andReturnValue(plan1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // First Task - Do not change

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // First Task - Complete

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->run();

    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task1, StaskPlannerMock->completedTaskList.front());

    // Second Task
    TaskMock* task2 = new TaskMock();
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue(plan2);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);

    Ssequencer->run();

    delete task2;
}
TEST(Sequencer, scenarioTaskSwitched)
{
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue((AMW_Task*) 0);

    Ssequencer->run();

    // First Task
    TaskMock* task1 = new TaskMock();
    CommandMock* plan1 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Task").expectOneCall("generatePlan").onObject(task1).andReturnValue(plan1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // First Task - Do not change

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // Switch to second task
    TaskMock* task2 = new TaskMock();
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan1);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasStarted").onObject(plan2).andReturnValue(false);

    Ssequencer->run();

    mock("Facade").checkExpectations();
    mock("Facade").clear();

    // Second task started
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasStarted").onObject(plan2).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("switchedToTask").onObject(StaskPlannerMock);

    Ssequencer->run();

    mock("TaskPlanner").checkExpectations();
    mock("TaskPlanner").clear();
    delete task1;

    // Second Task Completed
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->run();

    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task2, StaskPlannerMock->completedTaskList.front());
}
TEST(Sequencer, scenarioTaskSwitchFailed)
{
    // First Task
    TaskMock* task1 = new TaskMock();
    CommandMock* plan1 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Task").expectOneCall("generatePlan").onObject(task1).andReturnValue(plan1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // First Task - Do not change

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // Switch to second task
    TaskMock* task2 = new TaskMock();
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("pause").onObject(plan1);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasStarted").onObject(plan2).andReturnValue(false);

    Ssequencer->run();

    mock("Facade").checkExpectations();
    mock("Facade").clear();

    // Second Task Failed
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(true);
    mock("Command").expectOneCall("resume").onObject(plan1);

    Ssequencer->run();

    mock("TaskPlanner").checkExpectations();
    mock("TaskPlanner").clear();

    // First Task Completed
    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(true);
    mock("TaskPlanner").expectOneCall("completedTask").onObject(StaskPlannerMock);

    Ssequencer->run();

    CHECK_EQUAL(1, (int) StaskPlannerMock->completedTaskList.size());
    CHECK_EQUAL(task1, StaskPlannerMock->completedTaskList.front());

    CommandMock* plan3 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue(plan3);
    mock("Command").expectOneCall("run").onObject(plan3).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan3).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan3).andReturnValue(false);

    Ssequencer->run();

    delete task2;
}
TEST(Sequencer, scenarioTaskSwitchForced)
{
    // First Task
    TaskMock* task1 = new TaskMock();
    CommandMock* plan1 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Task").expectOneCall("generatePlan").onObject(task1).andReturnValue(plan1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // First Task - Do not change

    mock("TaskPlanner").setData("forceFirstTask", false);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task1);
    mock("Command").expectOneCall("run").onObject(plan1).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan1).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan1).andReturnValue(false);

    Ssequencer->run();

    // Force Switch to second task
    TaskMock* task2 = new TaskMock();
    CommandMock* plan2 = new CommandMock();

    mock("TaskPlanner").setData("forceFirstTask", true);
    mock("TaskPlanner").expectOneCall("getFirstTask").onObject(StaskPlannerMock).andReturnValue(task2);
    mock("Task").expectOneCall("generatePlan").onObject(task2).andReturnValue(plan2);
    mock("Facade").expectOneCall("loiter").onObject(SfacadeMock);
    mock("Command").expectOneCall("run").onObject(plan2).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(plan2).andReturnValue(false);
    mock("Command").expectOneCall("hasFailed").onObject(plan2).andReturnValue(false);

    Ssequencer->run();

    delete task1;
    delete task2;
}
