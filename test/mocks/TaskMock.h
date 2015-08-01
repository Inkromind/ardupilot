/*
 * TaskMock.h
 *
 *  Created on: 31-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_TASKMOCK_H_
#define MOCKS_TASKMOCK_H_

#include "../../libraries/AMW_Planner/AMW_Task.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class TaskMock : public AMW_Task {
public:
    virtual ~TaskMock() {};

    TaskMock() {
        taskId = 0;
    };

    AMW_Command* generatePlan(void) const {
        return (AMW_Command*) mock("Task").actualCall("generatePlan").onObject(const_cast<TaskMock*>(this)).returnPointerValue();
    };


    virtual Vector2f getStartPosition() const {
        return *((Vector2f*) mock("Task").actualCall("getStartPosition").onObject(const_cast<TaskMock*>(this))
                .returnPointerValue());
    }

    virtual Vector2f getEndPosition() const {
        return *((Vector2f*) mock("Task").actualCall("getEndPosition").onObject(const_cast<TaskMock*>(this))
                .returnPointerValue());
    }

    /**
     * Mark this task as completed
     */
    virtual void completedTask(void) {
        mock("Task").actualCall("completedTask").onObject(this);
    }
};

#endif /* MOCKS_TASKMOCK_H_ */
