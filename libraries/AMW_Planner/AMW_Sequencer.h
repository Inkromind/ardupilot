/*
 * AMW_Sequencer.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_SEQUENCER_H_
#define AMW_SEQUENCER_H_

#include "AMW_Task_Planner.h"
#include "AMW_Task.h"
#include "AMW_Commands_Plan.h"

class AMW_Sequencer {
public:
    virtual void run();
    virtual void init();

    static AMW_Sequencer* getInstance(void);

    void pauseMission(void);

    void toggleMission(void);

    void resumeMission(void);
private:

    bool sequencerInitialized;
    static AMW_Sequencer* sequencer;
    bool paused;

    AMW_Task* currentTask;
    AMW_Commands_Plan* currentPlan;

    AMW_Sequencer();
    virtual void startNewTask(void);
protected:
    virtual ~AMW_Sequencer();

};

#endif /* AMW_SEQUENCER_H_ */
