/*
 * AMW_Task_State_With_Subtask.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_STATE_WITH_SUBTASK_H_
#define AMW_TASK_STATE_WITH_SUBTASK_H_

#include "AMW_Task_State.h"
#include "AMW_Task.h"

class AMW_Task_State_With_Subtask : public AMW_Task_State {
public:
    virtual ~AMW_Task_State_With_Subtask() {
        if (subTask)
            delete subTask;
    }

    virtual void updateStatus(void) {
        if (!parentTask)
            return;

        subTask.updateStatus();

        if (subTask.isComplete())
            completed = true;
    }

    virtual void runTask(void) {
        if (!parentTask)
            return;

        if (completed)
            return;

        updateStatus();

        if (completed)
            return;

        subTask.runTask();
    }

    virtual bool isTaskComplete(void) {
        return false;
    }

    virtual void advanceState(void) {
        if (!parentTask)
            return;

        parentTask.setState(getNextState());
        delete this;
    }

    virtual bool isStateComplete(void) {
        return completed;
    }

private:
    bool running = false;
    bool completed = false;

    AMW_Task& subTask;

    virtual AMW_Task_State& getNextState(void) = 0;
};



#endif /* AMW_TASK_STATE_WITH_SUBTASK_H_ */
