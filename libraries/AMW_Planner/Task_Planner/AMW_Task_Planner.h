/*
 * AMW_Task_Planner.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_PLANNER_H_
#define AMW_TASK_PLANNER_H_

#include "AMW_Planner_Task.h"

class AMW_Task_Planner {
public:
    virtual void run(void);
    virtual void init(void);

    virtual AMW_Planner_Task* getFirstTask(void);
    virtual void completeFirstTask(void);

    static virtual AMW_Task_Planner* getInstance(void);
private:
    bool initialized = false;
    static AMW_Task_Planner planner;
    std::queue<AMW_Planner_Task*> plan;

    AMW_Task_Planner();
    virtual ~AMW_Task_Planner();

};

#endif /* AMW_TASK_PLANNER_H_ */
