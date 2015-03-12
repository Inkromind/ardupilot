/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Sequencer.h"

AMW_Sequencer::AMW_Sequencer() { }

AMW_Sequencer::~AMW_Sequencer() {
    delete currentTask;
}

static AMW_Sequencer* AMW_Sequencer::getInstance() {
    return &this->sequencer;
}

void AMW_Sequencer::init() {
    initialized = true;
}

void AMW_Sequencer::run() {
    if (!initialized)
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

    currentPlan = currentTask->generatePlan();

    run();
}
