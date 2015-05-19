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
    newTask = 0;
    sequencerInitialized = false;
    currentPlan = 0;
    paused = true;
}

AMW_Sequencer::~AMW_Sequencer() {
    delete currentTask;
}

AMW_Sequencer* AMW_Sequencer::getInstance() {
    if (!AMW_Sequencer::sequencer) {
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Creating Sequencer..."));
#endif
        AMW_Sequencer::sequencer = new AMW_Sequencer();
    }
    return AMW_Sequencer::sequencer;
}

void AMW_Sequencer::init() {
    if (sequencerInitialized) {
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
#ifdef AMW_PLANNER_DEBUG
    //AC_Facade::getFacade()->sendDebug(PSTR("Running Sequencer..."));
#endif
    
    newTask = AMW_Task_Planner::getInstance()->getFirstTask();
    
    if (!currentTask && !newTask) {
        return;
    }
    else if (newTask && newTask != currentTask) {
        startNewTask();
        return;
    }
    else {
        executeCurrentTask();
    }
}

void AMW_Sequencer::startNewTask() {
    if (!newTask)
        return;
    
    if (currentPlan)
        delete currentPlan;
    currentPlan = 0;

    currentTask = newTask;
    newTask = 0;

#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Got new task"));
#endif

    currentPlan = currentTask->generatePlan();
    if (!currentPlan) {
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("No Plan for task"));
#endif
        return;
    }

    executeCurrentTask();
}

void AMW_Sequencer::executeCurrentTask() {
    currentPlan->executePlan();
    if (currentPlan->isCompleted()) {
        delete currentPlan;
        currentPlan = 0;
        AMW_Task_Planner::getInstance()->completeFirstTask(currentTask);
        currentTask = 0;
        #ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Plan completed"));
        #endif
    }
    else if (currentPlan->hasFailed()) {
        // TODO: Abort Plan
        delete currentPlan;
        currentPlan = 0;
        AMW_Task_Planner::getInstance()->completeFirstTask(currentTask);
        currentTask = 0;
        #ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Plan failed"));
        #endif
    }
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
