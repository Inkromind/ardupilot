/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"

AMW_Task_Planner::AMW_Task_Planner() {
    // TODO Auto-generated constructor stub
}

AMW_Task_Planner::~AMW_Task_Planner() {
    while (!plan.empty()) {
        completeFirstTask();
    }
    delete plan;
}

static AMW_Task_Planner* AMW_Task_Planner::getInstance() {
    return &this->planner;
}

void AMW_Task_Planner::init() {
    initialized = true;
}

void AMW_Task_Planner::run() {
}


AMW_Planner_Task* AMW_Task_Planner::getFirstTask() {
    if (plan.empty())
        return nullptr;

    return plan.front();
}

void AMW_Task_Planner::completeFirstTask(void) {
    if (plan.empty())
        return;

    AMW_Planner_Task* task = plan.front();
    plan.pop();
    delete task;
}
