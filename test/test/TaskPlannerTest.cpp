/*
 * TaskPlannerTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */


//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Planner/AMW_Task_Planner.h"

//Mocks
#include  "../mocks/TaskMock.h"
#include  "../mocks/FacadeMock.h"
#include  "../mocks/BatteryMonitorMock.h"

//Includes


class DummyTaskPlanner : public AMW_Task_Planner {

public:
    ~DummyTaskPlanner() {
        AMW_Task_Planner::planner = 0;
    }
    bool isPaused() {
        return paused;
    }
    void setPaused(bool newVal) {
        paused = newVal;
    }
    bool isReturningHome() {
        return returningHome;
    }

    bool isIdleTaskCompleted() {
        return idleTaskCompleted;
    }
    void setIdleTaskCompleted(bool newVal) {
        idleTaskCompleted = newVal;
    }

    void setPlanner() {
        AMW_Task_Planner::planner = this;
    }


    AMW_List<AMW_Task*>* getPlan() {
        return &plan;
    }


    AMW_Task* getIdleTask() {
        return idleTask;
    }
    void setIdleTask(AMW_Task* task) {
        idleTask = task;
    }
};

DummyTaskPlanner* TPplanner = 0;
FacadeMock* TPfacadeMock = 0;
BattMonitorMock* TPbatteryMock = 0;

TEST_GROUP(TaskPlanner)
{
    void setup()
    {
        TPplanner = new DummyTaskPlanner();
        TPplanner->setPlanner();
        TPplanner->setPaused(false);
        TPplanner->init();
        TPplanner->setIdleTaskCompleted(false);
        TPfacadeMock = new FacadeMock();
        FacadeMock::setFacade(TPfacadeMock);
        TPbatteryMock = new BattMonitorMock();
    }
    void teardown()
    {
        FacadeMock::deleteFacade();
        delete TPfacadeMock;
        delete TPplanner;
        delete TPbatteryMock;
    }
};

TEST(TaskPlanner, getFirstTaskNotInitialized)
{
    DummyTaskPlanner planner;
    bool force = false;

    CHECK_EQUAL(0, planner.getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskBatteryEmpty)
{
    TPplanner->markBatteryEmpty();
    TPplanner->getPlan()->push_back(new TaskMock());
    bool force = false;

    CHECK_EQUAL(0, TPplanner->getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskReturningHomeCompleted)
{
    TPplanner->returnHome();
    TPplanner->setIdleTaskCompleted(true);
    TPplanner->getPlan()->push_back(new TaskMock());
    bool force = false;

    CHECK_EQUAL(0, TPplanner->getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskReturningHomeNotCompletedNoIdleTask)
{
    Vector2f home = Vector2f(1, 3);
    TPplanner->returnHome();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setHomeBase(home);
    bool force = false;

    AMW_Task* task = TPplanner->getFirstTask(&force);

    CHECK_TRUE(force);
    CHECK_TRUE(task);
    CHECK_EQUAL(8, task->taskId);
    CHECK_EQUAL(0, (home - task->getEndPosition()).length());
}
TEST(TaskPlanner, getFirstTaskReturningHomeNotCompletedIdleTask)
{
    TaskMock* task = new TaskMock();
    TPplanner->returnHome();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(task);
    bool force = false;

    CHECK_EQUAL(task, TPplanner->getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskPlanEmptyCompleted)
{
    TPplanner->setIdleTaskCompleted(true);
    bool force = false;

    CHECK_EQUAL(0, TPplanner->getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskPlanEmptyNotCompletedNoIdleTask)
{
    Vector2f home = Vector2f(1, 3);
    TPplanner->setHomeBase(home);
    bool force = false;

    AMW_Task* task = TPplanner->getFirstTask(&force);

    CHECK_TRUE(force);
    CHECK_TRUE(task);
    CHECK_EQUAL(9, task->taskId);
    CHECK_EQUAL(0, (home - task->getEndPosition()).length());
}
TEST(TaskPlanner, getFirstTaskPlanEmptyNotCompletedIdleTask)
{
    TaskMock* task = new TaskMock();
    TPplanner->setIdleTask(task);
    bool force = false;

    CHECK_EQUAL(task, TPplanner->getFirstTask(&force));

    CHECK_TRUE(force);
}
TEST(TaskPlanner, getFirstTaskNormal)
{
    TaskMock* idleTask = new TaskMock();
    TaskMock* task = new TaskMock();
    TPplanner->getPlan()->push_back(task);
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);
    bool force = false;

    CHECK_EQUAL(task, TPplanner->getFirstTask(&force));

    CHECK_FALSE(force);
    CHECK_EQUAL(task, TPplanner->getFirstTask(&force));

    CHECK_FALSE(force);
}
TEST(TaskPlanner, completedTaskNullTask)
{
    TaskMock* idleTask = new TaskMock();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);

    TPplanner->completedTask(0);

    CHECK_EQUAL(idleTask, TPplanner->getIdleTask());
    CHECK_EQUAL(1, TPplanner->getPlan()->size());
}
TEST(TaskPlanner, completedTaskNotMatching)
{
    TaskMock* idleTask = new TaskMock();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);
    TaskMock* task = new TaskMock();

    mock("Task").expectOneCall("completedTask").onObject(task);

    TPplanner->completedTask(task);

    CHECK_EQUAL(idleTask, TPplanner->getIdleTask());
    CHECK_EQUAL(1, TPplanner->getPlan()->size());
}
TEST(TaskPlanner, completedTaskMatchingInPlan)
{
    TaskMock* task = new TaskMock();
    TaskMock* task2 = new TaskMock();
    TaskMock* idleTask = new TaskMock();
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task);
    TPplanner->setIdleTask(idleTask);

    mock("Task").expectOneCall("completedTask").onObject(task);

    TPplanner->completedTask(task);

    CHECK_EQUAL(idleTask, TPplanner->getIdleTask());
    CHECK_EQUAL(task2, TPplanner->getPlan()->front());
}
TEST(TaskPlanner, completedTaskMatchingIdleTask)
{
    TaskMock* task = new TaskMock();
    TaskMock* task2 = new TaskMock();
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task);
    TPplanner->setIdleTask(task);

    mock("Task").expectOneCall("completedTask").onObject(task);

    TPplanner->completedTask(task);

    CHECK_EQUAL(0, TPplanner->getIdleTask());
    CHECK_EQUAL(task2, TPplanner->getPlan()->front());
}
TEST(TaskPlanner, completedTaskMatchingIdleTaskEmptyPlan)
{
    TaskMock* task = new TaskMock();
    TPplanner->getPlan()->push_back(task);
    TPplanner->setIdleTask(task);

    mock("Task").expectOneCall("completedTask").onObject(task);

    TPplanner->completedTask(task);

    CHECK_EQUAL(0, TPplanner->getIdleTask());
    CHECK_TRUE(TPplanner->getPlan()->empty());
    CHECK_TRUE(TPplanner->isIdleTaskCompleted());
}
TEST(TaskPlanner, completedTaskMatchingIdleTaskReturningHome)
{
    TPplanner->returnHome();
    TaskMock* task = new TaskMock();
    TaskMock* task2 = new TaskMock();
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task);
    TPplanner->setIdleTask(task);

    mock("Task").expectOneCall("completedTask").onObject(task);

    TPplanner->completedTask(task);

    CHECK_EQUAL(0, TPplanner->getIdleTask());
    CHECK_EQUAL(task2, TPplanner->getPlan()->front());
    CHECK_TRUE(TPplanner->isIdleTaskCompleted());
}
TEST(TaskPlanner, switchedToTaskNullTask)
{
    TaskMock* idleTask = new TaskMock();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);

    TPplanner->switchedToTask(0);

    CHECK_EQUAL(idleTask, TPplanner->getIdleTask());
    CHECK_EQUAL(1, TPplanner->getPlan()->size());
}
TEST(TaskPlanner, switchedToTaskReturningHome)
{
    TPplanner->returnHome();
    TaskMock* idleTask = new TaskMock();
    TaskMock* task = new TaskMock();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);

    TPplanner->switchedToTask(task);

    CHECK_EQUAL(idleTask, TPplanner->getIdleTask());
    CHECK_EQUAL(1, TPplanner->getPlan()->size());

    delete task;
}
TEST(TaskPlanner, switchedToTaskDifferentFromIdleTask)
{
    TPplanner->setIdleTaskCompleted(true);
    TaskMock* idleTask = new TaskMock();
    TaskMock* task = new TaskMock();
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->setIdleTask(idleTask);

    TPplanner->switchedToTask(task);

    CHECK_EQUAL(0, TPplanner->getIdleTask());
    CHECK_EQUAL(1, TPplanner->getPlan()->size());
    CHECK_FALSE(TPplanner->isIdleTaskCompleted());

    delete task;
}

TEST(TaskPlanner, addTaskNullTask)
{
    CHECK_EQUAL(0, TPplanner->addTask(0, false));

    CHECK_TRUE(TPplanner->getPlan()->empty());
}
TEST(TaskPlanner, addTaskEstimateBatteryEmpty)
{
    TPplanner->markBatteryEmpty();
    TaskMock task;
    task.taskId = 21;

    CHECK_EQUAL(0, TPplanner->addTask(&task, false));

    CHECK_TRUE(TPplanner->getPlan()->empty());
}
TEST(TaskPlanner, addTaskEstimateBatteryTooLow)
{
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->getPlan()->push_back(new TaskMock());
    TPplanner->getPlan()->push_back(new TaskMock());
    TaskMock task;
    task.taskId = 21;

    mock("Facade").expectOneCall("getBattery").onObject(TPfacadeMock).andReturnValue(TPbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(54);

    CHECK_EQUAL(0, TPplanner->addTask(&task, true));
    CHECK_EQUAL(3, TPplanner->getPlan()->size());
}
TEST(TaskPlanner, addTaskEmptyPlan)
{
    TaskMock* task = new TaskMock();
    task->taskId = 21;
    Vector2f homeBase = Vector2f(2,3);
    TPplanner->setHomeBase(homeBase);
    Vector2f startLocation = Vector2f(5, 6);
    Vector2f endLocation = Vector2f(10, 11);
    float expectedEstimate = (endLocation - startLocation).length() +
            (homeBase - startLocation).length() + (homeBase - endLocation).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task).andReturnValue(&startLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task).andReturnValue(&endLocation);

    float estimate = TPplanner->addTask(task, false);

    CHECK_EQUAL(expectedEstimate, estimate);
    CHECK_EQUAL(1, TPplanner->getPlan()->size());
    CHECK_EQUAL(task, TPplanner->getPlan()->front());
}
TEST(TaskPlanner, addTaskEmptyPlanEstimate)
{
    TaskMock* task = new TaskMock();
    task->taskId = 21;
    Vector2f homeBase = Vector2f(2,3);
    TPplanner->setHomeBase(homeBase);
    Vector2f startLocation = Vector2f(5, 6);
    Vector2f endLocation = Vector2f(10, 11);
    float expectedEstimate = (startLocation - endLocation).length() + (homeBase - startLocation).length()
            + (homeBase - endLocation).length();

    mock("Facade").expectOneCall("getBattery").onObject(TPfacadeMock).andReturnValue(TPbatteryMock);
    mock("BattMonitor").expectOneCall("batteryCapacity").andReturnValue(54);
    mock("Task").expectOneCall("getStartPosition").onObject(task).andReturnValue(&startLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task).andReturnValue(&endLocation);

    float estimate = TPplanner->addTask(task, true);

    CHECK_EQUAL(expectedEstimate, estimate);
    CHECK_TRUE(TPplanner->getPlan()->empty());
    delete task;
}
TEST(TaskPlanner, addTaskFrontOfPlan)
{
    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    TaskMock* task3 = new TaskMock();
    task3->taskId = 23;
    TaskMock* newTask = new TaskMock();
    newTask->taskId = 24;
    Vector2f homeBase = Vector2f(0,0);
    TPplanner->setHomeBase(homeBase);
    Vector2f newStartLocation = Vector2f(2, 4);
    Vector2f newEndLocation = Vector2f(6, 4);
    Vector2f startLocation2 = Vector2f(6, 5);
    Vector2f endLocation2 = Vector2f(9, 7);
    Vector2f startLocation3 = Vector2f(9, 7);
    Vector2f endLocation3 = Vector2f(10, 11);
    //Vector2f startLocation1 = Vector2f(2, 3);
    Vector2f endLocation1 = Vector2f(2, 3);
    TPplanner->getPlan()->push_back(task1);
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task3);
    float expectedEstimate = (newStartLocation - newEndLocation).length() + (endLocation1 - newStartLocation).length()
            + (newEndLocation - startLocation2).length() - (endLocation1 - startLocation2).length();

    mock("Task").expectOneCall("getStartPosition").onObject(newTask).andReturnValue(&newStartLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(newTask).andReturnValue(&newEndLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);
    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);
    mock("Task").expectOneCall("getStartPosition").onObject(task3).andReturnValue(&startLocation3);
    mock("Task").expectOneCall("getEndPosition").onObject(task3).andReturnValue(&endLocation3);

    float estimate = TPplanner->addTask(newTask, false);

    CHECK_EQUAL(expectedEstimate, estimate);
    CHECK_EQUAL(4, TPplanner->getPlan()->size());
    CHECK_EQUAL(newTask, TPplanner->getPlan()->get(1));
}
TEST(TaskPlanner, addTaskMiddleOfPlan)
{
    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    TaskMock* task3 = new TaskMock();
    task3->taskId = 23;
    TaskMock* newTask = new TaskMock();
    newTask->taskId = 24;
    Vector2f homeBase = Vector2f(0,0);
    TPplanner->setHomeBase(homeBase);
    //Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    Vector2f newStartLocation = Vector2f(7, 9);
    Vector2f newEndLocation = Vector2f(9, 9);
    Vector2f startLocation3 = Vector2f(10, 10);
    Vector2f endLocation3 = Vector2f(10, 11);
    Vector2f startLocation2 = Vector2f(3, 5);
    Vector2f endLocation2 = Vector2f(6, 5);
    TPplanner->getPlan()->push_back(task1);
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task3);
    float expectedEstimate = (newStartLocation - newEndLocation).length() +
            (endLocation2 - newStartLocation).length() + (newEndLocation - startLocation3).length() -
            (endLocation2 - startLocation3).length();

    mock("Task").expectOneCall("getStartPosition").onObject(newTask).andReturnValue(&newStartLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(newTask).andReturnValue(&newEndLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);
    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);
    mock("Task").expectOneCall("getStartPosition").onObject(task3).andReturnValue(&startLocation3);
    mock("Task").expectOneCall("getEndPosition").onObject(task3).andReturnValue(&endLocation3);

    float estimate = TPplanner->addTask(newTask, false);

    CHECK_EQUAL(expectedEstimate, estimate);
    CHECK_EQUAL(4, TPplanner->getPlan()->size());
    CHECK_EQUAL(newTask, TPplanner->getPlan()->get(2));
}
TEST(TaskPlanner, addTaskAlreadyAdded)
{
    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    TaskMock* task3 = new TaskMock();
    task3->taskId = 23;
    TaskMock* newTask = new TaskMock();
    newTask->taskId = 22;
    Vector2f homeBase = Vector2f(0,0);
    TPplanner->setHomeBase(homeBase);
    //Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    Vector2f newStartLocation = Vector2f(7, 9);
    Vector2f newEndLocation = Vector2f(9, 9);
    TPplanner->getPlan()->push_back(task1);
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task3);

    mock("Task").expectOneCall("getStartPosition").onObject(newTask).andReturnValue(&newStartLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(newTask).andReturnValue(&newEndLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);

    float estimate = TPplanner->addTask(newTask, false);

    CHECK_EQUAL(0, estimate);
    CHECK_EQUAL(3, TPplanner->getPlan()->size());

    delete newTask;
}
TEST(TaskPlanner, addTaskBackOfPlan)
{
    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    TaskMock* task3 = new TaskMock();
    task3->taskId = 23;
    TaskMock* newTask = new TaskMock();
    newTask->taskId = 24;
    Vector2f homeBase = Vector2f(0,0);
    TPplanner->setHomeBase(homeBase);
    //Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    Vector2f startLocation2 = Vector2f(3, 5);
    Vector2f endLocation2 = Vector2f(6, 5);
    Vector2f startLocation3 = Vector2f(7, 9);
    Vector2f endLocation3 = Vector2f(10, 11);
    Vector2f newStartLocation = Vector2f(12, 14);
    Vector2f newEndLocation = Vector2f(1, 9);
    TPplanner->getPlan()->push_back(task1);
    TPplanner->getPlan()->push_back(task2);
    TPplanner->getPlan()->push_back(task3);
    float expectedEstimate = (newStartLocation - newEndLocation).length() +
            (endLocation3 - newStartLocation).length() + (newEndLocation - homeBase).length() -
            (endLocation3 - homeBase).length();

    mock("Task").expectOneCall("getStartPosition").onObject(newTask).andReturnValue(&newStartLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(newTask).andReturnValue(&newEndLocation);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);
    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);
    mock("Task").expectOneCall("getStartPosition").onObject(task3).andReturnValue(&startLocation3);
    mock("Task").expectOneCall("getEndPosition").onObject(task3).andReturnValue(&endLocation3);

    float estimate = TPplanner->addTask(newTask, false);

    CHECK_EQUAL(expectedEstimate, estimate);
    CHECK_EQUAL(4, TPplanner->getPlan()->size());
    CHECK_EQUAL(newTask, TPplanner->getPlan()->get(3));
}

TEST(TaskPlanner, ScenarioNormal)
{
    Vector2f homebase = Vector2f(2,0);
    TPplanner->setHomeBase(homebase);
    bool forceTask = false;

    AMW_Task* idleTask = TPplanner->getFirstTask(&forceTask);

    CHECK_TRUE(idleTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TPplanner->completedTask(idleTask);

    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    float expectedEstimate = (endLocation1 - startLocation1).length() +
            (homebase - startLocation1).length() + (homebase - endLocation1).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task1).andReturnValue(&startLocation1);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);

    float estimate = TPplanner->addTask(task1, false);

    CHECK_EQUAL(expectedEstimate, estimate);

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    Vector2f startLocation2 = Vector2f(6, 7);
    Vector2f endLocation2 = Vector2f(8, 9);
    expectedEstimate = (startLocation2 - endLocation2).length() +
            (endLocation1 - startLocation2).length() + (endLocation2 - homebase).length() -
            (endLocation1 - homebase).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);

    estimate = TPplanner->addTask(task2, false);

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    TaskMock* task3 = new TaskMock();
    task3->taskId = 23;
    Vector2f startLocation3 = Vector2f(3, 4);
    Vector2f endLocation3 = Vector2f(5, 6);
    expectedEstimate = (startLocation3 - endLocation3).length() +
            (endLocation1 - startLocation3).length() + (endLocation3 - startLocation2).length() -
            (endLocation1 - startLocation2).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task3).andReturnValue(&startLocation3);
    mock("Task").expectOneCall("getEndPosition").onObject(task3).andReturnValue(&endLocation3);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);
    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);

    estimate = TPplanner->addTask(task3, false);

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    mock("Task").expectOneCall("completedTask").onObject(task1);

    TPplanner->completedTask(task1);

    CHECK_EQUAL(task3, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    mock("Task").expectOneCall("completedTask").onObject(task3);

    TPplanner->completedTask(task3);

    CHECK_EQUAL(task2, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    mock("Task").expectOneCall("completedTask").onObject(task2);

    TPplanner->completedTask(task2);

    idleTask = TPplanner->getFirstTask(&forceTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TPplanner->completedTask(idleTask);
}
TEST(TaskPlanner, ScenarioNewPackageWhileIdleReturning)
{
    Vector2f homebase = Vector2f(2,0);
    TPplanner->setHomeBase(homebase);
    bool forceTask = false;

    AMW_Task* idleTask = TPplanner->getFirstTask(&forceTask);

    CHECK_TRUE(idleTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TPplanner->completedTask(idleTask);

    TaskMock* task1 = new TaskMock();
    task1->taskId = 21;
    Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    float expectedEstimate = (endLocation1 - startLocation1).length() +
            (homebase - startLocation1).length() + (homebase - endLocation1).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task1).andReturnValue(&startLocation1);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);

    float estimate = TPplanner->addTask(task1, false);

    CHECK_EQUAL(expectedEstimate, estimate);

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    mock("Task").expectOneCall("completedTask").onObject(task1);

    TPplanner->completedTask(task1);

    idleTask = TPplanner->getFirstTask(&forceTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TaskMock* task2 = new TaskMock();
    task2->taskId = 22;
    Vector2f startLocation2 = Vector2f(6, 7);
    Vector2f endLocation2 = Vector2f(8, 9);
    expectedEstimate = (endLocation2 - startLocation1).length() +
                (homebase - startLocation2).length() + (homebase - endLocation2).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);

    estimate = TPplanner->addTask(task2, false);

    CHECK_EQUAL(task2, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);
    CHECK_EQUAL(9, idleTask->taskId);

    TPplanner->completedTask(idleTask);

    CHECK_EQUAL(task2, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);
}
TEST(TaskPlanner, ScenarioRTS)
{
    Vector2f homebase = Vector2f(2,0);
    TPplanner->setHomeBase(homebase);
    bool forceTask = false;

    AMW_Task* idleTask = TPplanner->getFirstTask(&forceTask);

    CHECK_TRUE(idleTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TPplanner->completedTask(idleTask);

    TaskMock* task1 = new TaskMock();
    task1->taskId = 20;
    Vector2f startLocation1 = Vector2f(1, 1);
    Vector2f endLocation1 = Vector2f(2, 3);
    float expectedEstimate = (endLocation1 - startLocation1).length() +
            (homebase - startLocation1).length() + (homebase - endLocation1).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task1).andReturnValue(&startLocation1);
    mock("Task").expectOneCall("getEndPosition").onObject(task1).andReturnValue(&endLocation1);

    float estimate = TPplanner->addTask(task1, false);

    CHECK_EQUAL(expectedEstimate, estimate);

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    TPplanner->returnHome();

    idleTask = TPplanner->getFirstTask(&forceTask);

    CHECK_TRUE(idleTask);
    CHECK_EQUAL(8, idleTask->taskId);
    CHECK_TRUE(forceTask);

    mock("Task").expectOneCall("completedTask").onObject(task1);

    TPplanner->completedTask(idleTask);

    CHECK_EQUAL(0, TPplanner->getFirstTask(&forceTask));
    CHECK_TRUE(forceTask);

    TPplanner->resumeMission();

    CHECK_EQUAL(task1, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);

    TPplanner->completedTask(task1);

    idleTask = TPplanner->getFirstTask(&forceTask);
    CHECK_EQUAL(9, idleTask->taskId);
    CHECK_TRUE(forceTask);

    TaskMock* task2 = new TaskMock();
    task2->taskId = 21;
    Vector2f startLocation2 = Vector2f(6, 7);
    Vector2f endLocation2 = Vector2f(8, 9);
    expectedEstimate = (endLocation2 - startLocation1).length() +
                (homebase - startLocation2).length() + (homebase - endLocation2).length();

    mock("Task").expectOneCall("getStartPosition").onObject(task2).andReturnValue(&startLocation2);
    mock("Task").expectOneCall("getEndPosition").onObject(task2).andReturnValue(&endLocation2);

    estimate = TPplanner->addTask(task2, false);

    CHECK_EQUAL(task2, TPplanner->getFirstTask(&forceTask));
    CHECK_FALSE(forceTask);
    CHECK_EQUAL(9, idleTask->taskId);

    TPplanner->completedTask(idleTask);
}
