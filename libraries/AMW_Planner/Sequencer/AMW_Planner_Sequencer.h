/*
 * AMW_Sequencer.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_SEQUENCER_H_
#define AMW_SEQUENCER_H_

#include "AMW_Task_Planner.h"
#include "AMW_Planner_Task.h"

class AMW_Sequencer {
public:
    virtual void run();
    virtual void init();
private:

    bool initialized = false;
    static AMW_Sequencer sequencer;

    AMW_Planner_Task* currentTask = 0;

    AMW_Sequencer();
    virtual ~AMW_Sequencer();

    static virtual AMW_Sequencer* getInstance(void);

    virtual void startNewTask(void);
};

#endif /* AMW_SEQUENCER_H_ */
