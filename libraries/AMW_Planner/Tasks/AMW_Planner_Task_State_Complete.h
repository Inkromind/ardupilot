/*
 * AMW_Planner_Task_State_Complete.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_TASK_STATE_COMPLETE_H_
#define AMW_PLANNER_TASK_STATE_COMPLETE_H_

#include "AMW_Planner_Task_State.h"

class AMW_Planner_Task_State_Complete : public AMW_Planner_Task_State {
public:

    virtual void updateStatus(void) { }

    virtual void runTask(void) { }

    virtual bool isTaskComplete(void) {
        return true;
    }

    virtual bool isStateComplete(void) {
        return false;
    }

    virtual void advanceState(void) { }
};



#endif /* AMW_PLANNER_TASK_STATE_COMPLETE_H_ */
