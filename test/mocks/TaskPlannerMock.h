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


    std::list<AMW_Task*> completeFirstTaskList;
    std::list<AMW_Task*> switchedToFirstTaskList;

    virtual ~TaskPlannerMock() {
        while (!completeFirstTaskList.empty()) {
            delete completeFirstTaskList.front();
            completeFirstTaskList.pop_front();
        }
    }

    void markBatteryEmpty(void) {
        mock("TaskPlanner").actualCall("markBatteryEmpty").onObject(this);
    }

    AMW_Task* getFirstTask(bool* forceTask) {
        *forceTask = mock("TaskPlanner").getData("forceFirstTask").getIntValue();
        return (AMW_Task*) mock("TaskPlanner").actualCall("getFirstTask").onObject(this).returnPointerValue();
    }

    void completeFirstTask(AMW_Task* task) {
        completeFirstTaskList.push_back(task);
        mock("TaskPlanner").actualCall("completeFirstTask").onObject(this);
    }

    void switchedToFirstTask(AMW_Task* task) {
        switchedToFirstTaskList.push_back(task);
        mock("TaskPlanner").actualCall("switchedToFirstTask").onObject(this);
    }
};



#endif /* MOCKS_TASKPLANNERMOCK_H_ */
