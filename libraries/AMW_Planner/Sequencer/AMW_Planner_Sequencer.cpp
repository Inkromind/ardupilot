/*
 * AMW_Sequencer.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Planner_Sequencer.h"

AMW_Sequencer::AMW_Sequencer() {
    // TODO Auto-generated constructor stub

}

AMW_Sequencer::~AMW_Sequencer() {
    // TODO Auto-generated destructor stub
}

static AMW_Sequencer* AMW_Sequencer::getInstance() {
    return &this->sequencer;
}

void AMW_Sequencer::init() {
    initialized = true;
}

void AMW_Sequencer::run() {
    if (!currentTask) {
        startNewTask();
    }
    else {
        currentTask->updateStatus();
        if (currentTask->isComplete()) {
            AMW_Task_Planner::getInstance()->completeFirstTask();
            startNewTask();
        }
    }
}

void AMW_Sequencer::startNewTask() {
    currentTask = AMW_Task_Planner::getInstance()->getFirstTask();

    if (!currentTask)
        return;

    currentTask->runTask();
}
