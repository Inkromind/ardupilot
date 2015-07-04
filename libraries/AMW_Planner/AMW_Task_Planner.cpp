/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"
#include "AMW_Task_Test_Flight.h"
#include "AMW_Task_Test_Flight_2.h"
#include "AMW_Task_RTL.h"
#include "AMW_Planner.h"
#include <AC_Facade.h>

AMW_Task_Planner* AMW_Task_Planner::planner = 0;

AMW_Task_Planner::AMW_Task_Planner() {
    plannerInitialized = false;
    paused = true;
    homeBase = Vector2f();
    idleTask = 0;
    assignedAltitude = 1500;
    returningHome = false;
}

AMW_Task_Planner::~AMW_Task_Planner() {
    while (!plan.empty()) {
        delete plan.front();
        plan.pop_front();
    }
}

AMW_Task_Planner* AMW_Task_Planner::getInstance() {
    if (!AMW_Task_Planner::planner) {
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Creating Task Planner..."));
#endif
        AMW_Task_Planner::planner = new AMW_Task_Planner();
    }
    return AMW_Task_Planner::planner;
}

void AMW_Task_Planner::init() {
    plannerInitialized = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Initializing Task Planner..."));
#endif
}

void AMW_Task_Planner::run() {
    if (!plannerInitialized)
        return;
    return;
}


AMW_Task* AMW_Task_Planner::getFirstTask() {
    if (!plannerInitialized)
        return 0;

    if (plan.empty() || returningHome)
        return idleTask;

    return plan.front();
}

void AMW_Task_Planner::completeFirstTask(AMW_Task* task) {
    if (!plannerInitialized)
        return;

    if (plan.empty() || returningHome) {
        if (idleTask && idleTask == task) {
            idleTask->completeTask();
            delete idleTask;
            idleTask = 0;
        }
        return;
    }

    AMW_Task* firstTask = plan.front();
    if (firstTask != task)
        return;

    firstTask->completeTask();
    plan.pop_front();
    delete firstTask;

    if (plan.empty() && !idleTask)
        idleTask = new AMW_Task_RTL();
}

void AMW_Task_Planner::pauseMission() {
    paused = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Pausing Task Planner"));
#endif
}

void AMW_Task_Planner::returnHome() {
    returningHome = true;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Returning Home"));
#endif
    if (!idleTask)
        idleTask = new AMW_Task_RTL();
    if (!paused)
        pauseMission();
}

void AMW_Task_Planner::resumeMission(void) {
    paused = false;
    if (returningHome && idleTask) {
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Cancelling return to home"));
#endif
        delete idleTask;
        idleTask = 0;
    }
    returningHome = false;
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Resuming Task Planner"));
#endif
}

void AMW_Task_Planner::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
}

void AMW_Task_Planner::addTask(AMW_Task *task) {
    plan.push_back(task);
    if (idleTask && !returningHome) {
        delete idleTask;
        idleTask = 0;
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendDebug(PSTR("Deleted idleTask"));
#endif
    }
}

float AMW_Task_Planner::addPackage(AMW_Task_Package *package, bool estimate) {
    uint32_t currentIndex = 0;
    uint32_t bestPosition = 1;
    Vector2f position = homeBase;
    Vector2f pickupLocation = package->pickupLocation;
    Vector2f deliveryLocation = package->deliveryLocation;
    float minDistance = -1;

    AMW_List<AMW_Task*>::Iterator* iterator = plan.iterator();
    while (iterator->hasNext()) {
#ifdef AMW_PLANNER_DEBUG
    //AC_Facade::getFacade()->sendDebug(PSTR("Checking next index"));
#endif
        AMW_Task *nextTask = iterator->next();
        if (currentIndex > 0) {
            float distance =
                    (position - pickupLocation).length() +
                    (deliveryLocation - nextTask->getStartPosition(position)).length() -
                    (position - nextTask->getStartPosition(position)).length();
            if (minDistance == -1 || distance <= minDistance) {
                minDistance = distance;
                bestPosition = currentIndex;
            }
        }
        currentIndex++;
        position = nextTask->getEndPosition(position);
    }
    float distance = (position - pickupLocation).length() + (deliveryLocation - homeBase).length() - (position - homeBase).length();
    if (minDistance == -1 || distance <= minDistance) {
        minDistance = distance;
        bestPosition = currentIndex;
    }

    if (!estimate) {
        plan.insert(bestPosition, package);
        if (idleTask && !returningHome) {
            delete idleTask;
            idleTask = 0;
#ifdef AMW_PLANNER_DEBUG
            AC_Facade::getFacade()->sendDebug(PSTR("Deleted idleTask"));
#endif
        }
#ifdef AMW_PLANNER_DEBUG
        AC_Facade::getFacade()->sendFormattedDebug(PSTR("Added new package #%d at index %d/%d"), package->id, bestPosition, plan.size());
#endif
    }

    return minDistance;
}
