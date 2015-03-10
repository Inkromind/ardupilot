/*
 * AMW_Planner_Task_State_With_Subtask.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_TASK_STATE_WITH_SUBTASK_H_
#define AMW_PLANNER_TASK_STATE_WITH_SUBTASK_H_

#include "AMW_Planner_Task_State.h"
#include "AMW_Planner_Task.h"

class AMW_Planner_Task_State_With_Subtask : public AMW_Planner_Task_State {
public:
    virtual ~AMW_Planner_Task_State_With_Subtask() {
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

    virtual AMW_Planner_Task_State& getNextState(void) = 0;

private:
    bool running = false;
    bool completed = false;

    AMW_Planner_Task& subTask;
};



#endif /* AMW_PLANNER_TASK_STATE_WITH_SUBTASK_H_ */
