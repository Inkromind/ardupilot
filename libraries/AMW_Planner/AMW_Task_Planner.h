/*
 * AMW_Task_Planner.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_PLANNER_H_
#define AMW_TASK_PLANNER_H_

#include "AMW_Task.h"
#include <AMW_Queue.h>

class AMW_Task_Planner {
public:

    virtual void run(void);
    virtual void init(void);

    void pauseMission(void);
    void toggleMission(void);
    void resumeMission(void);

    virtual AMW_Task* getFirstTask(void);
    virtual void completeFirstTask(void);

    static AMW_Task_Planner* getInstance(void);
private:
    bool plannerInitialized;
    bool paused;
    static AMW_Task_Planner* planner;
    AMW_Queue<AMW_Task*> plan;

    AMW_Task_Planner();
protected:
    virtual ~AMW_Task_Planner();

};

#endif /* AMW_TASK_PLANNER_H_ */
