/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"

AMW_Task_Planner::AMW_Task_Planner() {
    plan.push(new AMW_Task_Takeoff(2000));
    plan.push(new AMW_Task_Delay(20000));
    plan.push(new AMW_Task_Nav(Vector3f(2000, 2000, 0)));
    plan.push(new AMW_Task_Nav(Vector3f(5000, 2000, 0)));
    plan.push(new AMW_Task_Nav(Vector3f(5000, 2000, 5000)));
    plan.push(new AMW_Task_Nav(Vector3f(5000, 1000, 5000)));
    plan.push(new AMW_Task_Nav(Vector3f(0, 4500, 0)));
    plan.push(new AMW_Task_Land());
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
