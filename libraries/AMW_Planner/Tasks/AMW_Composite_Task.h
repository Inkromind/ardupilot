/*
 * AMW_Composite_Task.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMPOSITE_TASK_H_
#define AMW_COMPOSITE_TASK_H_

#include "AMW_Task.h"
#include "AMW_Task_State.h"

class AMW_Composite_Task : public AMW_Task {
public:
    virtual ~AMW_Composite_Task() {
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

    virtual void setState(AMW_Task_State& state) {
        currentState = state;
    }

private:
    AMW_Task_State* currentState;
};



#endif /* AMW_COMPOSITE_TASK_H_ */
