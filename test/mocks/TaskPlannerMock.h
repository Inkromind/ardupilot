/*
 * TaskPlannerMock.h
 *
 *  Created on: 30-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_TASKPLANNERMOCK_H_
#define MOCKS_TASKPLANNERMOCK_H_

#include "../../libraries/AMW_Planner/AMW_Task_Planner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <list>

class TaskPlannerMock : public AMW_Task_Planner {
public:
    static void setPlanner(TaskPlannerMock* planner) {
        AMW_Task_Planner::planner = planner;
    }
    static void deletePlanner() {
        AMW_Task_Planner::planner = 0;
    }


    std::list<AMW_Task*> completedTaskList;
    std::list<AMW_Task*> switchedToTaskList;

    virtual ~TaskPlannerMock() {
        while (!completedTaskList.empty()) {
            delete completedTaskList.front();
            completedTaskList.pop_front();
        }
    }

    void markBatteryEmpty(void) {
        mock("TaskPlanner").actualCall("markBatteryEmpty").onObject(this);
    }

    AMW_Task* getFirstTask(bool* forceTask) {
        *forceTask = mock("TaskPlanner").getData("forceFirstTask").getIntValue();
        return (AMW_Task*) mock("TaskPlanner").actualCall("getFirstTask").onObject(this).returnPointerValue();
    }

    void completedTask(AMW_Task* task, bool failed) {
        completedTaskList.push_back(task);
        mock("TaskPlanner").actualCall("completedTask").onObject(this);
    }

    void switchedToTask(AMW_Task* task) {
        switchedToTaskList.push_back(task);
        mock("TaskPlanner").actualCall("switchedToTask").onObject(this);
    }
};



#endif /* MOCKS_TASKPLANNERMOCK_H_ */
