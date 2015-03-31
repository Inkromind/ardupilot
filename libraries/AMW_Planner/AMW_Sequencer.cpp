/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Sequencer.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Sequencer* AMW_Sequencer::sequencer = 0;

AMW_Sequencer::AMW_Sequencer() {
    currentTask = 0;
    sequencerInitialized = false;
    currentPlan = 0;
    paused = true;
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
    if (AMW_Sequencer::sequencerInitialized) {
        return;
    }
    sequencerInitialized = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Initializing Sequencer..."));
#endif
}

void AMW_Sequencer::run() {
    if (!sequencerInitialized) {
        return;
    }

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
            currentPlan = 0;
            currentTask = 0;
#ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Plan completed"));
#endif
            AMW_Task_Planner::getInstance()->completeFirstTask();
        }
        else if (currentPlan->hasFailed()) {
            // TODO: Abort Plan
            delete currentPlan;
            currentPlan = 0;
            currentTask = 0;
            AMW_Task_Planner::getInstance()->completeFirstTask();
#ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Plan failed"));
#endif
        }
    }
}

void AMW_Sequencer::startNewTask() {
    currentTask = AMW_Task_Planner::getInstance()->getFirstTask();

    if (!currentTask)
        return;

#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Got next task"));
#endif

    currentPlan = currentTask->generatePlan();
    if (!currentPlan) {
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("No Plan for task"));
#endif
        return;
    }

    run();
}

void AMW_Sequencer::pauseMission() {
    if (paused)
        return;
    paused = true;
    AC_Facade::getFacade()->loiter();
    if (currentPlan)
        currentPlan->pausePlan();
}

void AMW_Sequencer::resumeMission(void) {
    if (!paused)
        return;
    paused = false;
    if (currentPlan)
        currentPlan->resumePlan();
}

void AMW_Sequencer::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
}
