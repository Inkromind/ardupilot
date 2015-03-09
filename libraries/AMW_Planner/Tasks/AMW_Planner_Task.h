/*
 * AMW_Planner_Task.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_TASK_H_
#define AMW_PLANNER_TASK_H_

class AMW_Planner_Task {
public:
    virtual ~AMW_Planner_Task();

    virtual void updateStatus(void) = 0;

    virtual void runTask(void) = 0;

    virtual bool isComplete(void);

private:
    bool running = false;
    bool completed = false;
    AMW_Task_Command* currentCommand;
};

#endif /* AMW_PLANNER_TASK_H_ */
