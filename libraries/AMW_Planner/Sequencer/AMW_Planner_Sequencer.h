/*
 * AMW_Planner_Sequencer.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_SEQUENCER_H_
#define AMW_PLANNER_SEQUENCER_H_

#include "AMW_Task_Planner.h"
#include "AMW_Planner_Task.h"

class AMW_Planner_Sequencer {
public:
    virtual void run();
    virtual void init();
private:

    bool initialized = false;
    static AMW_Planner_Sequencer sequencer;

    AMW_Planner_Task* currentTask = 0;

    AMW_Planner_Sequencer();
    virtual ~AMW_Planner_Sequencer();

    static virtual AMW_Planner_Sequencer* getInstance(void);

    virtual void startNewTask(void);
};

#endif /* AMW_PLANNER_SEQUENCER_H_ */
