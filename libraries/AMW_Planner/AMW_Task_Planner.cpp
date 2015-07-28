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
#include <AC_CommunicationFacade.h>

AMW_Task_Planner* AMW_Task_Planner::planner = 0;

AMW_Task_Planner::AMW_Task_Planner() {
    plannerInitialized = false;
    paused = true;
    homeBase = Vector2f();
    idleTask = 0;
    assignedAltitude = 1500;
    returningHome = false;
    idleTaskCompleted = false;
    batteryEmpty = false;
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
    AC_CommunicationFacade::sendDebug(PSTR("Creating Task Planner..."));
#endif
        AMW_Task_Planner::planner = new AMW_Task_Planner();
    }
    return AMW_Task_Planner::planner;
}

void AMW_Task_Planner::init() {
    plannerInitialized = true;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Initializing Task Planner..."));
#endif
}

void AMW_Task_Planner::run() {
    if (!plannerInitialized)
        return;
    return;
}


AMW_Task* AMW_Task_Planner::getFirstTask(bool* forceTask) {
    if (!plannerInitialized || isBatteryEmpty()) {
        *forceTask = true;
        return 0;
    }

    if (returningHome || plan.empty()) {
        *forceTask = true;
        if (!idleTaskCompleted)
            return getIdleTask();
        else
            return 0;
    }
    else {
        *forceTask = false;
        return plan.front();
    }
}

void AMW_Task_Planner::completeFirstTask(AMW_Task* task) {
    if (!task)
        return;

    task->completeTask();

    uint32_t id = plan.size();
    while (id > 0) {
        id--;
        AMW_Task* nextTask = plan.get(id);
        if (task == nextTask)
            plan.erase(id);
    }

    if (idleTask == task) {
        idleTask = 0;
        if (plan.empty() || returningHome)
            idleTaskCompleted = true;
    }


    delete task;
}

void AMW_Task_Planner::pauseMission() {
    paused = true;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Pausing Task Planner"));
#endif
}

void AMW_Task_Planner::returnHome() {
    returningHome = true;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Returning Home"));
#endif
}

void AMW_Task_Planner::resumeMission(void) {
    paused = false;
#ifdef AMW_PLANNER_DEBUG
    if (returningHome && !plan.empty()) {
        AC_CommunicationFacade::sendDebug(PSTR("Cancelling return to home"));
    }
#endif
    returningHome = false;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Resuming Task Planner"));
#endif
}

void AMW_Task_Planner::toggleMission(void) {
    if (paused)
        resumeMission();
    else
        pauseMission();
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
    //AC_CommunicationFacade::sendDebug(PSTR("Checking next index"));
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
        if (!returningHome)
            idleTaskCompleted = false;
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Added new package #%d at index %d/%d"), package->id, bestPosition, plan.size());
#endif
    }

    return minDistance;
}

void AMW_Task_Planner::markBatteryEmpty(void) {
    batteryEmpty = true;
}

void AMW_Task_Planner::switchedToFirstTask(AMW_Task* task) {
    if (!task)
        return;
    if (returningHome)
        return;
    else if (idleTask != task && idleTask) {
        delete idleTask;
        idleTask = 0;
        idleTaskCompleted = false;
    }
}

AMW_Task* AMW_Task_Planner::getIdleTask() {
    if (!idleTask)
        idleTask = new AMW_Task_RTL();
    return idleTask;
}
