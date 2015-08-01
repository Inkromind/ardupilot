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
    Ssequencer->setTempTask(&task);
    Ssequencer->setCurrentTask(&task2);
    Ssequencer->setExecutingCurrentTask(true);

    Ssequencer->runTryNewTask(&task);

    CHECK_EQUAL(&task, Ssequencer->getTempTask());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runTryNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runStartNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runStartNewTask(task3);

    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task3, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteCurrentTask();

    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteCurrentTask();

    CHECK_EQUAL(0, Ssequencer->getCurrentTask());
    CHECK_EQUAL(0, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("completeFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteTempTask();

    CHECK_EQUAL(&task2, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan2, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_EQUAL(1, (int) StaskPlannerMock->completeFirstTaskList.size());
    CHECK_EQUAL(task, StaskPlannerMock->completeFirstTaskList.front());
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
    mock("TaskPlanner").expectOneCall("switchedToFirstTask").onObject(StaskPlannerMock);

    Ssequencer->runExecuteTempTask();

    CHECK_EQUAL(&task, Ssequencer->getCurrentTask());
    CHECK_EQUAL(plan, Ssequencer->getCurrentPlan());
    CHECK_EQUAL(0, Ssequencer->getTempTask());
    CHECK_EQUAL(0, Ssequencer->getTempPlan());
    CHECK_TRUE(Ssequencer->isExecutingCurrentTask());
    CHECK_EQUAL(1, (int) StaskPlannerMock->switchedToFirstTaskList.size());
    CHECK_EQUAL(&task, StaskPlannerMock->switchedToFirstTaskList.front());
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

}
TEST(Sequencer, resumeMissionForceExecutingAsTempWithPlan)
{

}
TEST(Sequencer, resumeMissionForceExecutingAsCurrentNoPlan)
{

}
TEST(Sequencer, resumeMissionForceExecutingAsCurrentWithPlan)
{

}
TEST(Sequencer, resumeMissionForceCurrentTaskNotExecutingPlan)
{

}
TEST(Sequencer, resumeMissionForceCurrentTaskNotExecutingNoPlan)
{

}
TEST(Sequencer, resumeMissionForceNewTask)
{

}
TEST(Sequencer, resumeMissionNoFirstTask)
{

}
TEST(Sequencer, resumeMissionExecutingAsTempTaskPlan)
{

}
TEST(Sequencer, resumeMissionExecutingAsTempTaskNoPlan)
{

}
TEST(Sequencer, resumeMissionExecutingAsCurrentTaskPlan)
{

}
TEST(Sequencer, resumeMissionExecutingAsCurrentTaskNoPlan)
{

}
TEST(Sequencer, resumeMissionNewTask)
{

}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTaskNoCurrentPlan)
{

}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTaskNoTempPlan)
{

}
TEST(Sequencer, checkExecutingTaskNoFirstTaskForceTask)
{

}
TEST(Sequencer, checkExecutingTaskNoCurrentTask)
{

}
TEST(Sequencer, checkExecutingTaskForceTask)
{

}
TEST(Sequencer, checkExecutingTaskDifferentFromCurrentDifferentFromTemp)
{

}
TEST(Sequencer, checkExecutingTaskDifferentFromCurrentSameAsTemp)
{

}
TEST(Sequencer, checkExecutingTaskSameAsCurrentNew)
{

}
TEST(Sequencer, checkExecutingTaskSameAsCurrentNoNew)
{

}
TEST(Sequencer, runNoCurrentPlanExecutingCurrentTask)
{

}
TEST(Sequencer, runCurrentPlanExecutingCurrentTask)
{

}
TEST(Sequencer, runCurrentPlanNotExecutingCurrentTask)
{

}
TEST(Sequencer, runTempPlanExecutingCurrentTask)
{

}
TEST(Sequencer, runNoTempPlanExecutingCurrentTask)
{

}
TEST(Sequencer, runNoTempPlanNotExecutingCurrentTask)
{

}
TEST(Sequencer, scenarioNormal)
{

}
TEST(Sequencer, scenarioTaskSwitched)
{

}
TEST(Sequencer, scenarioTaskSwitchFailed)
{

}
TEST(Sequencer, scenarioTaskSwitchForced)
{

}
