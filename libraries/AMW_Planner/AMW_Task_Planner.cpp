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
    if (!plannerInitialized || paused)
        return;
    return;
}


AMW_Task* AMW_Task_Planner::getFirstTask() {
    if (!plannerInitialized || paused)
        return 0;

    if (plan.empty())
        return idleTask;

    return plan.front();
}

void AMW_Task_Planner::completeFirstTask(AMW_Task* task) {
    if (!plannerInitialized)
        return;

    if (plan.empty()) {
        if (idleTask && idleTask == task) {
            delete idleTask;
            idleTask = 0;
        }
        return;
    }

    AMW_Task* firstTask = plan.front();
    if (firstTask != task)
        return;

    plan.pop_front();
    delete firstTask;

    if (plan.empty())
        idleTask = new AMW_Task_RTL();
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

void AMW_Task_Planner::addTask(AMW_Task *task) {
    plan.push_back(task);
    if (idleTask) {
        delete idleTask;
        idleTask = 0;
    }
}

float AMW_Task_Planner::addPackage(AMW_Task_Package *package, bool estimate) {
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Attempting to add new package"));
#endif
    uint32_t currentIndex = 0;
    uint32_t bestPosition = 1;
    Vector2f position = homeBase;
    Vector2f pickupLocation = package->pickupLocation;
    Vector2f deliveryLocation = package->deliveryLocation;
    float minDistance = -1;

    AMW_List<AMW_Task*>::Iterator* iterator = plan.iterator();
    while (iterator->hasNext()) {
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendDebug(PSTR("Checking next index"));
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
    float distance = (position - pickupLocation).length() + (deliveryLocation - homeBase).length();
    if (minDistance == -1 || distance <= minDistance) {
        minDistance = distance;
        bestPosition = currentIndex;
    }

    if (!estimate) {
        plan.insert(bestPosition, package);
        if (idleTask) {
            delete idleTask;
            idleTask = 0;
        }
#ifdef AMW_PLANNER_DEBUG
        char debugbuffer[50];
        snprintf(debugbuffer, 50, "Added new package #%d at index %d", package->id, bestPosition);
        AC_Facade::getFacade()->sendDebug(PSTR(debugbuffer));
#endif
    }

    return minDistance;
}
