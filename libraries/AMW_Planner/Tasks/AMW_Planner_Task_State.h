/*
 * AMW_Planner_Task_State.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_TASK_STATE_H_
#define AMW_PLANNER_TASK_STATE_H_

class AMW_Planner_Task_State {
public:
    virtual ~AMW_Planner_Task_State();

    virtual void updateStatus(void) = 0;

    virtual void runTask(void) = 0;

    virtual bool isTaskComplete(void) = 0;

    virtual bool isStateComplete(void) = 0;

    virtual void advanceState(void) = 0;

private:
    AMW_Planner_State_Task& parentTask;
};


#endif /* AMW_PLANNER_TASK_STATE_H_ */
