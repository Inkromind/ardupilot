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
#include "AMW_Task_RTL_Forced.h"
#include "AMW_Planner.h"
#include <AC_CommunicationFacade.h>

AMW_Task_Planner* AMW_Task_Planner::planner = 0;

AMW_Task_Planner::AMW_Task_Planner() {
    plannerInitialized = false;
    paused = true;
    homeBase = Vector2f();
    idleTask = 0;
    returningHome = false;
    idleTaskCompleted = false;
    batteryEmpty = false;
}

AMW_Task_Planner::~AMW_Task_Planner() {
    while (!plan.empty()) {
        delete plan.front();
        plan.pop_front();
    }
    if (idleTask)
        delete idleTask;
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
            return fetchIdleTask();
        else
            return 0;
    }
    else {
        idleTaskCompleted = false;
        *forceTask = false;
        return plan.front();
    }
}

void AMW_Task_Planner::completedTask(AMW_Task* task) {
    if (!task)
        return;

    task->completedTask();

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
    delete idleTask;
#ifdef AMW_PLANNER_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Returning Home"));
#endif
    paused = false;
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

float AMW_Task_Planner::addTask(AMW_Task *task, bool estimate) {
    if (!task || batteryEmpty)
        return 0;

    if (estimate) {
        uint8_t remainingBattery = AC_Facade::getFacade()->getBattery()->capacity_remaining_pct();
        uint8_t batteryEstimate = (plan.size() + 2) * 5;
        if (remainingBattery - batteryEstimate < AMW_PLANNER_BATTERY_TAKEOFF_LIMIT)
            return 0;
    }

    uint32_t currentIndex = 0;
    uint32_t bestPosition = 1;
    Vector2f position = homeBase;
    Vector2f startLocation = task->getStartPosition();
    if (startLocation.is_nan())
        startLocation = homeBase;
    Vector2f endLocation = task->getEndPosition();
    if (endLocation.is_nan())
        endLocation = homeBase;
    float packageDistance = (endLocation - startLocation).length();
    float minDistance = -1;

    AMW_List<AMW_Task*>::Iterator* iterator = plan.iterator();
    while (iterator->hasNext()) {
#ifdef AMW_PLANNER_DEBUG
    //AC_CommunicationFacade::sendDebug(PSTR("Checking next index"));
#endif
        AMW_Task *nextTask = iterator->next();
        if (currentIndex > 0) {
            Vector2f nextStartPosition = nextTask->getStartPosition();
            if (nextStartPosition.is_nan())
                nextStartPosition = position;
            float distance = packageDistance +
                    (position - startLocation).length() +
                    (endLocation - nextStartPosition).length() -
                    (position - nextStartPosition).length();
            if (minDistance == -1 || distance <= minDistance) {
                minDistance = distance;
                bestPosition = currentIndex;
            }
        }
        currentIndex++;
        Vector2f endPosition = nextTask->getEndPosition();
        if (!endPosition.is_nan())
            position = endPosition;
    }
    delete iterator;
    float distance = (position - startLocation).length() + packageDistance + (endLocation - homeBase).length() - (position - homeBase).length();
    if (minDistance == -1 || distance <= minDistance) {
        minDistance = distance;
        bestPosition = currentIndex;
    }

    if (!estimate) {
        plan.insert(bestPosition, task);
#ifdef AMW_PLANNER_DEBUG
        AC_CommunicationFacade::sendFormattedDebug(PSTR("Added new task #%d at index %d/%d"), task->id, bestPosition, plan.size());
#endif
    }

    return minDistance;
}

void AMW_Task_Planner::markBatteryEmpty(void) {
    batteryEmpty = true;
}

void AMW_Task_Planner::switchedToTask(AMW_Task* task) {
    if (!task)
        return;
    if (returningHome)
        return;
    else if (idleTask != task) {
        delete idleTask;
        idleTask = 0;
        idleTaskCompleted = false;
    }
}

AMW_Task* AMW_Task_Planner::fetchIdleTask() {
    if (!idleTask) {
        if (returningHome)
            idleTask = new AMW_Task_RTL_Forced();
        else
            idleTask = new AMW_Task_RTL();
    }
    return idleTask;
}
