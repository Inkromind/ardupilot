/*
 * AMW_Commands_Plan.cpp
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Commands_Plan.h"

AMW_Commands_Plan::AMW_Commands_Plan(AMW_Task& parentTask) {
    this->parentTask = parentTask;
}

AMW_Commands_Plan::~AMW_Commands_Plan(void) {
    while (!plan.empty()) {
        delete plan.front();
        plan.pop();
    }
}

void AMW_Commands_Plan::executePlan(void) {
    if (completed || failed)
        return;
    if (currentCommand) {
        currentCommand->runCommand();
        if (currentCommand->isComplete()) {
            completeCurrentCommand();
            executePlan();
        }
    }
    else if (plan.empty()) {
        completed = true;
        return;
    }
    else {
        currentCommand = plan.front();
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
