/*
 * MAD_MissionPlanner.cpp
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#include "MAD_MissionPlanner.h"
#include "MAD_Task.h"
#include "AC_Facade.h"

MAD_MissionPlanner::~MAD_MissionPlanner() {
    // TODO Auto-generated destructor stub
}

// Default constructor.
MAD_MissionPlanner::MAD_MissionPlanner() {
    // TODO Auto-generated constructor stub
}

void MAD_MissionPlanner::init() {
    this->initialized = true;
}

static MAD_MissionPlanner* MAD_MissionPlanner::getInstance() {
    return &this->planner;
}

void MAD_MissionPlanner::run() {

    if (!this->initialized) {
        return;
    }
    // Check if the mission is still on
    if (!AC_Facade::inControl()) {
        return;
    }

    if (this->currentTask) {

        if (this->currentTask->isComplete()) {
            delete this->currentTask;
            this->currentTask = 0;
            startNextTask();
        }
    }
    else {
        startNextTask();
    }
}

void MAD_MissionPlanner::startNextTask() {
    if (this->taskQueue.empty()) {
        return;
    }
    this->currentTask = this->taskQueue.pop();
    this->currentTask->runTask();
}
