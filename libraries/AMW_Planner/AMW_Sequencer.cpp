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

    resetLogging();
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
#ifdef AMW_PLANNER_DEBUG
            if (currentPlan || tempPlan)
                AC_CommunicationFacade::sendDebug(PSTR("Forced cancelling of task. Loitering..."));
#endif
            AC_Facade::getFacade()->loiter();
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
    else if (!currentTask) {
        startNewTask(firstTask);
    }
    else if (firstTask != currentTask) {
        if (forceTask) {
            AC_Facade::getFacade()->loiter();
            startNewTask(firstTask);
        }
        else if (tempTask != firstTask) {
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
    if (!currentTask) {
        startNewTask(task);
        return;
    }
    if (task == tempTask && tempPlan)
        return;
    tempTask = task;
    executingCurrentTask = false;

    if (tempPlan)
        delete tempPlan;
    tempPlan = tempTask->generatePlan();


    if (!tempPlan) {
        executingCurrentTask = true;
        AMW_Task_Planner::getInstance()->completedTask(tempTask);
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
        AC_CommunicationFacade::sendDebug(PSTR("Trying to switch to new task. Pausing old task"));
#endif
    }

}

void AMW_Sequencer::startNewTask(AMW_Task* task) {
    if (!task) {
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("Starting Loitering..."));
#endif
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



    currentPlan = currentTask->generatePlan();
    if (!currentPlan) {
        AMW_Task_Planner::getInstance()->completedTask(currentTask);
        currentTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendDebug(PSTR("No Plan to start new task"));
#endif
    }
    else {
#ifdef AMW_PLANNER_DEBUG
        //AC_CommunicationFacade::sendDebug(PSTR("Starting new task"));
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
        AMW_Task_Planner::getInstance()->completedTask(currentTask);
        currentTask = 0;
        #ifdef AMW_PLANNER_DEBUG
            //AC_CommunicationFacade::sendDebug(PSTR("Plan completed"));
        #endif
    }
    else if (currentPlan->hasFailed()) {
        delete currentPlan;
        currentPlan = 0;
        AMW_Task_Planner::getInstance()->completedTask(currentTask, true);
        currentTask = 0;
        #ifdef AMW_PLANNER_DEBUG
            //AC_CommunicationFacade::sendDebug(PSTR("Plan failed"));
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
        AMW_Task_Planner::getInstance()->completedTask(tempTask);
        tempTask = 0;
#ifdef AMW_PLANNER_DEBUG
        //AC_CommunicationFacade::sendDebug(PSTR("New Plan completed"));
#endif
    }
    else if (tempPlan->hasFailed()) {
#ifdef AMW_PLANNER_DEBUG
        //AC_CommunicationFacade::sendDebug(PSTR("New Plan failed"));
#endif
        delete tempPlan;
        tempPlan = 0;
        executingCurrentTask = true;
        if (currentPlan)
            currentPlan->resume();
    }
    else if (tempPlan->hasStarted()) {
        delete currentPlan;
        currentPlan = tempPlan;
        currentTask = tempTask;
        tempPlan = 0;
        tempTask = 0;
        executingCurrentTask = true;
        AMW_Task_Planner::getInstance()->switchedToTask(currentTask);
    }
}

void AMW_Sequencer::cancelTempTaskContinueCurrentTask(bool removeNewTask) {
    delete tempPlan;
    tempPlan = 0;

    if (currentTask) {
        if (currentPlan) {
#ifdef AMW_PLANNER_DEBUG
            if (!executingCurrentTask)
                AC_CommunicationFacade::sendDebug(PSTR("Cancelling new task. Continuing old task"));
#endif
            currentPlan->resume();
        }
        else
            startNewTask(currentTask);
    }
    if (removeNewTask) {
        tempTask = 0;
    }
    executingCurrentTask = true;
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
#ifdef AMW_PLANNER_DEBUG
                AC_CommunicationFacade::sendDebug(PSTR("Resuming new task"));
#endif
                delete currentPlan;
                currentTask = tempTask;
                currentPlan = tempPlan;
                tempTask = 0;
                tempPlan = 0;
                currentPlan->resume();
                executingCurrentTask = true;
                AMW_Task_Planner::getInstance()->switchedToTask(currentTask);
            }
        }
        else if (currentTask == firstTask) {
            cancelTempTaskContinueCurrentTask(true);
        }
        else
            startNewTask(firstTask);
    }
    else if (!firstTask) {
        if (!executingCurrentTask && tempTask) {
            if (tempPlan) {
#ifdef AMW_PLANNER_DEBUG
                AC_CommunicationFacade::sendDebug(PSTR("Resuming new task"));
#endif
                tempPlan->resume();
            }
            else
                tryNewTask(tempTask);
        }
        else if (currentTask) {
            if (currentPlan) {
#ifdef AMW_PLANNER_DEBUG
                AC_CommunicationFacade::sendDebug(PSTR("Resuming old task"));
#endif
                currentPlan->resume();
            }
            else
                startNewTask(currentTask);
        }
    }
    else if (tempTask == firstTask && !executingCurrentTask) {
        if (tempPlan) {
#ifdef AMW_PLANNER_DEBUG
            AC_CommunicationFacade::sendDebug(PSTR("Resuming new task"));
#endif
            tempPlan->resume();
        }
        else
            tryNewTask(firstTask);
    }
    else if (currentTask == firstTask)
        cancelTempTaskContinueCurrentTask(true);
    else
        tryNewTask(firstTask);
}

void AMW_Sequencer::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
}

void AMW_Sequencer::resetLogging() {
    totalReturnToStarts = 0;
    totalEmergencyLandings = 0;
    completedPackages = 0;
    failedPackages = 0;
}
