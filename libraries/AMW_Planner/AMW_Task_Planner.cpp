/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"
#include "AMW_Task_Test_Flight.h"
#include "AMW_Task_Test_Flight_2.h"

AMW_Task_Planner* AMW_Task_Planner::planner = 0;

AMW_Task_Planner::AMW_Task_Planner() {
    plannerInitialized = false;
    paused = true;
    plan.push(new AMW_Task_Test_Flight_2());
    plan.push(new AMW_Task_Test_Flight());
    plan.push(new AMW_Task_Test_Flight_2());
    plan.push(new AMW_Task_Test_Flight());
    plan.push(new AMW_Task_Test_Flight_2());
    plan.push(new AMW_Task_Test_Flight());
    plan.push(new AMW_Task_Test_Flight_2());
    plan.push(new AMW_Task_Test_Flight());
    plan.push(new AMW_Task_Test_Flight_2());
    plan.push(new AMW_Task_Test_Flight());
}

AMW_Task_Planner::~AMW_Task_Planner() {
    while (!plan.empty()) {
        completeFirstTask();
    }
}

AMW_Task_Planner* AMW_Task_Planner::getInstance() {
    if (!planner)
      planner = new AMW_Task_Planner();
    return planner;
}

void AMW_Task_Planner::init() {
    plannerInitialized = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Initializing Task Planner..."));
#endif
}

void AMW_Task_Planner::run() {
    if (!plannerInitialized || paused)
        return;
}


AMW_Task* AMW_Task_Planner::getFirstTask() {
    if (!plannerInitialized || paused || plan.empty())
        return 0;

    return plan.front();
}

void AMW_Task_Planner::completeFirstTask() {
    if (plan.empty())
        return;

    delete plan.front();
    plan.pop();
}

void AMW_Task_Planner::pauseMission() {
    paused = true;
}

void AMW_Task_Planner::resumeMission(void) {
    paused = false;
}

void AMW_Task_Planner::toggleMission(void) {
    paused = !paused;
}
