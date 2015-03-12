/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"
#include <AMW_Task_Test_Flight.h>

AMW_Task_Planner::AMW_Task_Planner() {
    plan.push(new AMW_Task_Test_Flight());
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
    if (!initialized)
        return;
}


AMW_Task* AMW_Task_Planner::getFirstTask() {
    if (!initialized || plan.empty())
        return nullptr;

    return plan.front();
}

void AMW_Task_Planner::completeFirstTask(void) {
    if (plan.empty())
        return;

    delete plan.front();
    plan.pop();
}
