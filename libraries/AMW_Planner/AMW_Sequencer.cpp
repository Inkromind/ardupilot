/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Sequencer.h"

AMW_Sequencer* AMW_Sequencer::sequencer = 0;

AMW_Sequencer::AMW_Sequencer() {
    currentTask = 0;
    sequencerInitialized = false;
    currentPlan = 0;
}

AMW_Sequencer::~AMW_Sequencer() {
    delete currentTask;
}

AMW_Sequencer* AMW_Sequencer::getInstance() {
    if (!AMW_Sequencer::sequencer) {
        AMW_Sequencer::sequencer = new AMW_Sequencer();
    }
    return AMW_Sequencer::sequencer;
}

void AMW_Sequencer::init() {
    sequencerInitialized = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::sendDebug(PSTR("Initializing Sequencer..."));
#endif
}

void AMW_Sequencer::run() {
    if (!sequencerInitialized)
        return;

    if (!currentTask) {
        startNewTask();
        return;
    }
    else if (!currentPlan) {
        startNewTask();
        return;
    }
    else {
        currentPlan->executePlan();
        if (currentPlan->isCompleted()) {
            delete currentPlan;
            AMW_Task_Planner::getInstance()->completeFirstTask();
        }
        else if (currentPlan->hasFailed()) {
            // TODO: Abort Plan
            delete currentPlan;
            AMW_Task_Planner::getInstance()->completeFirstTask();
        }
    }
}

void AMW_Sequencer::startNewTask() {
    currentTask = AMW_Task_Planner::getInstance()->getFirstTask();

    if (!currentTask)
        return;

#ifdef AMW_PLANNER_DEBUG
    AC_Facade::sendDebug(PSTR("Got next task"));
#endif

    currentPlan = currentTask->generatePlan();

    run();
}
