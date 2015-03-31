/*
 * AMW_Commands_Plan.cpp
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Commands_Plan.h"
#include "AMW_Planner.h"
#include <AC_Facade.h>

AMW_Commands_Plan::AMW_Commands_Plan() {
    this->failed = false;
    this->completed = false;
    this->currentCommand = 0;
    this->paused = false;
}

AMW_Commands_Plan::~AMW_Commands_Plan(void) {
    while (!this->plan.empty()) {
        delete this->plan.front();
        this->plan.pop();
    }
}

void AMW_Commands_Plan::executePlan(void) {
    if (completed || failed || paused)
        return;
    if (currentCommand) {
        currentCommand->runCommand();
        if (currentCommand->isComplete()) {
#ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Command completed"));
#endif
            completeCurrentCommand();
            executePlan();
        }
    }
    else if (plan.empty()) {
        completed = true;
        return;
    }
    else {
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Got next command"));
#endif
        currentCommand = plan.front();
        executePlan();
    }

}

bool AMW_Commands_Plan::isCompleted(void) {
    return completed;
}

bool AMW_Commands_Plan::hasFailed(void) {
    return failed;
}

void AMW_Commands_Plan::completeCurrentCommand(void) {
    plan.pop();
    delete currentCommand;
    currentCommand = 0;
}

void AMW_Commands_Plan::addNewCommand(AMW_Command* command) {
    if (!command)
        return;
    plan.push(command);
}

void AMW_Commands_Plan::pausePlan() {
    if (paused)
        return;
    paused = true;
    if (currentCommand)
        currentCommand->pauseCommand();
}

void AMW_Commands_Plan::resumePlan() {
    if (!paused)
        return;
    paused = false;
    if (currentCommand)
        currentCommand->resumeCommand();
}
