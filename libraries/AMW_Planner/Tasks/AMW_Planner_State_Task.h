/*
 * AMW_Planner_Composite_Task.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_COMPOSITE_TASK_H_
#define AMW_PLANNER_COMPOSITE_TASK_H_

#include "AMW_Planner_Task.h"
#include "AMW_Planner_Task_State.h"

class AMW_Planner_State_Task : public AMW_Planner_Task {
public:
    virtual ~AMW_Planner_State_Task() {
        if (currentState)
            delete currentState;
    }

    virtual void updateStatus(void) {
        if (completed)
            return;

        if (!currentState) {
            completed = true;
        }

        currentState->updateStatus();
        if (currentState->isStateComplete()) {
            currentState->advanceState();
        }
        if (currentState->isTaskComplete()) {
            completed = true;
        }
    }

    virtual void runTask(void) {
        if (completed)
            return;
        if (!currentState) {
            completed = true;
            return;
        }

        currentState->runTask();
    }

    virtual void setState(AMW_Planner_Task_State& state) {
        currentState = state;
    }

private:
    AMW_Planner_Task_State* currentState;
};



#endif /* AMW_PLANNER_COMPOSITE_TASK_H_ */
