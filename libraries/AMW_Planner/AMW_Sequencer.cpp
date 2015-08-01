/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Sequencer.h"
#include <AC_Facade.h>
#include <AC_CommunicationFacade.h>
#include "AMW_Planner.h"
#include "AMW_Task_Planner.h"

AMW_Sequencer* AMW_Sequencer::sequencer = 0;

AMW_Sequencer::AMW_Sequencer() {
    currentTask = 0;
    tempTask = 0;
    sequencerInitialized = false;
    currentPlan = 0;
    tempPlan = 0;
    paused = true;
    executingCurrentTask = true;
}

AMW_Sequencer::~AMW_Sequencer() {
    if (currentPlan) {
        delete currentPlan;
        currentPlan = 0;
    }
    if (tempPlan) {
        delete tempPlan;
        tempPlan = 0;
    }
}

AMW_Sequencer* AMW_Sequencer::getInstance() {
    if (!AMW_Sequencer::sequencer) {
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Creating Sequencer..."));
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
    AC_CommunicationFacade::sendDebug(PSTR("Initializing Sequencer..."));
#endif
}

void AMW_Sequencer::run() {
    if (!sequencerInitialized) {
        return;
    }
#ifdef AMW_PLANNER_DEBUG
    //AC_CommunicationFacade::sendDebug(PSTR("Running Sequencer..."));
#endif
    
    checkExecutingTask();

    if (currentPlan && executingCurrentTask) {
        executeCurrentTask();
    }
    else if (tempPlan && !executingCurrentTask) {
        executeTempTask();
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
            if (tempPlan) {
                delete tempPlan;
                tempPlan = 0;
            }
            currentTask = 0;
            tempTask = 0;
        }
    }
    else if (!currentTask || forceTask) {
        startNewTask(firstTask);
    }
    else if (firstTask != currentTask) {
        if (tempTask != firstTask) {
            tryNewTask(firstTask);
        }
    }
    else if (tempTask || !executingCurrentTask) {
        cancelTempTaskContinueCurrentTask(true);
    }

}

void AMW_Sequencer::tryNewTask(AMW_Task* task) {
    if (!task)
        return;
    if (task == tempTask)
        return;

    tempTask = task;
    executingCurrentTask = false;

#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Got new task replacing old task"));
#endif

    if (tempPlan)
        delete tempPlan;
    tempPlan = tempTask->generatePlan();


    if (!tempPlan) {
        executingCurrentTask = true;
        AMW_Task_Planner::getInstance()->completeFirstTask(tempTask);
        tempTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("No Plan for new task, continuing old task"));
#endif
    }
    else {
        AC_Facade::getFacade()->loiter();
        if (currentPlan)
            currentPlan->pause();

#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Paused current plan"));
#endif
    }

}

void AMW_Sequencer::startNewTask(AMW_Task* task) {
    if (!task) {
        AC_Facade::getFacade()->loiter();
        delete currentPlan;
        currentPlan = 0;
        delete tempPlan;
        tempPlan = 0;
        currentTask = 0;
        tempTask = 0;
        return;
    }
    
    if (currentPlan) {
        delete currentPlan;
        currentPlan = 0;
    }
    if (tempPlan) {
        delete tempPlan;
        tempPlan = 0;
    }
    tempTask = 0;
    currentTask = task;
    executingCurrentTask = true;

#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Got new task"));
#endif

    currentPlan = currentTask->generatePlan();
    if (!currentPlan) {
        AMW_Task_Planner::getInstance()->completeFirstTask(currentTask);
        currentTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("No Plan for task"));
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
            AC_CommunicationFacade::sendDebug(PSTR("Plan completed"));
        #endif
    }
    else if (currentPlan->hasFailed()) {
        // TODO: Abort Plan
        delete currentPlan;
        currentPlan = 0;
        AMW_Task_Planner::getInstance()->completeFirstTask(currentTask);
        currentTask = 0;
        #ifdef AMW_PLANNER_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Plan failed"));
        #endif
    }
}

void AMW_Sequencer::executeTempTask() {
    if (paused)
        return;

    tempPlan->run(true);
    if (tempPlan->isComplete()) {
        delete tempPlan;
        tempPlan = 0;
        AMW_Task_Planner::getInstance()->completeFirstTask(tempTask);
        tempTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("New Plan completed"));
#endif
    }
    else if (tempPlan->hasFailed()) {
        delete tempPlan;
        tempPlan = 0;
    }
    else if (tempPlan->hasStarted()) {
        delete currentPlan;
        currentPlan = tempPlan;
        currentTask = tempTask;
        tempPlan = 0;
        tempTask = 0;
        executingCurrentTask = true;
        AMW_Task_Planner::getInstance()->switchedToFirstTask(currentTask);
    }
}

void AMW_Sequencer::cancelTempTaskContinueCurrentTask(bool removeNewTask) {
    delete tempPlan;
    tempPlan = 0;
    executingCurrentTask = true;
    if (currentTask) {
        if (currentPlan)
            currentPlan->resume();
        else
            startNewTask(currentTask);
    }
    if (removeNewTask) {
        tempTask = 0;
    }
}

void AMW_Sequencer::pauseMission() {
    if (paused)
        return;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Pausing Sequencer"));
#endif
    paused = true;
    AC_Facade::getFacade()->loiter();
    if (currentPlan && executingCurrentTask)
        currentPlan->pause();
    else if (tempPlan && !executingCurrentTask)
        tempPlan->pause();
}

void AMW_Sequencer::resumeMission(void) {
    if (!paused)
        return;
    paused = false;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Resuming Sequencer"));
#endif
    bool force = false;
    AMW_Task* firstTask = AMW_Task_Planner::getInstance()->getFirstTask(&force);
    if (force) {
        if (!firstTask)
            startNewTask(firstTask);
        else if (!executingCurrentTask && tempTask == firstTask) {
            if (!tempPlan) {
                startNewTask(firstTask);
            }
            else {
                delete currentPlan;
                currentTask = tempTask;
                currentPlan = tempPlan;
                tempTask = 0;
                tempPlan = 0;
                currentPlan->resume();
                executingCurrentTask = true;
                AMW_Task_Planner::getInstance()->switchedToFirstTask(currentTask);
            }
        }
        else if (currentTask == firstTask) {
            cancelTempTaskContinueCurrentTask(true);
        }
        else
            startNewTask(firstTask);
    }
    else if (!firstTask)
        return;
    else if (tempTask == firstTask && !executingCurrentTask) {
        if (tempPlan)
            tempPlan->resume();
        else
            tryNewTask(firstTask);
    }
    else if (currentTask == firstTask && executingCurrentTask)
        cancelTempTaskContinueCurrentTask(true);
    else
        tryNewTask(firstTask);
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Resuming plan"));
#endif
}

void AMW_Sequencer::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
}
