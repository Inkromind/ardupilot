/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Sequencer.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"
#include "AMW_Task_Planner.h"

AMW_Sequencer* AMW_Sequencer::sequencer = 0;

AMW_Sequencer::AMW_Sequencer() {
    currentTask = 0;
    newTask = 0;
    sequencerInitialized = false;
    currentPlan = 0;
    newPlan = 0;
    paused = true;
    executingCurrentTask = true;
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
    
    checkExecutingTask();

    if (currentPlan && executingCurrentTask) {
        executeCurrentTask();
    }
    else if (newPlan && !executingCurrentTask) {
        executeNewTask();
    }
}

void AMW_Sequencer::checkExecutingTask() {
    bool forceTask = false;
    AMW_Task* firstTask = AMW_Task_Planner::getInstance()->getFirstTask(&forceTask);

    if (!firstTask) {
        if (forceTask) {
            executingCurrentTask = true;
            if (currentPlan) {
                delete currentPlan;
                currentPlan = 0;
            }
            if (newPlan) {
                delete newPlan;
                newPlan = 0;
            }
            currentTask = 0;
            newTask = 0;
        }
    }
    else if (!currentTask || forceTask) {
        startNewTask(firstTask);
    }
    else if (firstTask != currentTask) {
        if (newTask != firstTask) {
            tryNewTask(firstTask);
        }
    }
    else if (newTask) {
        cancelNewTaskContinueCurrentTask(true);
    }

}

void AMW_Sequencer::tryNewTask(AMW_Task* task) {
    if (!task)
        return;
    if (task == newTask)
        return;

    newTask = task;
    executingCurrentTask = false;

#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Got new task replacing old task"));
#endif

    if (newPlan)
        delete newPlan;
    newPlan = newTask->generatePlan();


    if (!newPlan) {
        executingCurrentTask = true;
        AMW_Task_Planner::getInstance()->completeFirstTask(newTask);
        newTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("No Plan for new task, continuing old task"));
#endif
    }
    else {
        AC_Facade::getFacade()->loiter();
        if (currentPlan)
            currentPlan->pause();

#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Paused current plan"));
#endif
    }

}

void AMW_Sequencer::startNewTask(AMW_Task* task) {
    if (!task)
        return;
    
    if (currentPlan)
        delete currentPlan;
    if (newPlan)
        delete newPlan;
    newTask = 0;
    currentTask = task;
    executingCurrentTask = true;

#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Got new task"));
#endif

    currentPlan = currentTask->generatePlan();
    if (!currentPlan) {
        AMW_Task_Planner::getInstance()->completeFirstTask(currentTask);
        currentTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("No Plan for task"));
#endif
    }
}

void AMW_Sequencer::executeCurrentTask() {
    if (paused)
        return;

    currentPlan->run(false);
    if (currentPlan->isComplete()) {
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

void AMW_Sequencer::executeNewTask() {
    if (paused)
        return;

    newPlan->run(true);
    if (newPlan->isComplete()) {
        delete newPlan;
        newPlan = 0;
        AMW_Task_Planner::getInstance()->completeFirstTask(newTask);
        newTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("New Plan completed"));
#endif
    }
    else if (newPlan->hasFailed()) {
        delete newPlan;
        newPlan = 0;
        cancelNewTaskContinueCurrentTask(false);
    }
    else if (newPlan->hasStarted()) {
        delete currentPlan;
        currentPlan = newPlan;
        AMW_Task_Planner::getInstance()->switchedToFirstTask(newTask);
        currentTask = newTask;
        newPlan = 0;
        newTask = 0;
        executingCurrentTask = true;
    }
}

void AMW_Sequencer::cancelNewTaskContinueCurrentTask(bool removeNewTask) {
    delete newPlan;
    newPlan = 0;
    if (currentPlan)
        currentPlan->resume();
    if (removeNewTask) {
        newTask = 0;
    }
}

void AMW_Sequencer::pauseMission() {
    if (paused)
        return;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Pausing Sequencer"));
#endif
    paused = true;
    AC_Facade::getFacade()->loiter();
    if (currentPlan)
        currentPlan->pause();
}

void AMW_Sequencer::resumeMission(void) {
    if (!paused)
        return;
    paused = false;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Resuming Sequencer"));
#endif
    if (currentPlan) {
        newTask = AMW_Task_Planner::getInstance()->getFirstTask();

        if (currentTask && currentTask == newTask) {
#ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Resuming plan"));
#endif
            currentPlan->resume();
            return;
        }
        newTask = 0;
    }
}

void AMW_Sequencer::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
}
