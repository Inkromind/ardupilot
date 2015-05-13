/*
 * AMW_Task_Planner.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Planner.h"
#include "AMW_Task_Package.h"
#include "AMW_Task_Test_Flight.h"
#include "AMW_Task_Test_Flight_2.h"

AMW_Task_Planner* AMW_Task_Planner::planner = 0;

AMW_Task_Planner::AMW_Task_Planner() {
    plannerInitialized = false;
    paused = true;
    homeBase = new Vector3f();
    assignedAltitude = 1500;
    plan.push_back(new AMW_Task_Test_Flight_2());
    plan.push_back(new AMW_Task_Test_Flight());
    plan.push_back(new AMW_Task_Test_Flight_2());
    plan.push_back(new AMW_Task_Test_Flight());
    plan.push_back(new AMW_Task_Test_Flight_2());
    plan.push_back(new AMW_Task_Test_Flight());
    plan.push_back(new AMW_Task_Test_Flight_2());
    plan.push_back(new AMW_Task_Test_Flight());
    plan.push_back(new AMW_Task_Test_Flight_2());
    plan.push_back(new AMW_Task_Test_Flight());
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
    plan.pop_front();
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
}

float AMW_Task_Planner::addPackage(AMW_Task_Package *package, bool estimate = false) {
    uint32_t currentIndex = 0;
    uint32_t bestPosition = 1;
    Vector3f position = homeBase;
    Vector3f pickupLocation = Vector3f(package->pickupLocation.x, package->pickupLocation.y, 0);
    Vector3f deliveryLocation = Vector3f(package->deliveryLocation.x, package->deliveryLocation.y, 0);
    float minDistance = -1;

    AMW_List<AMW_Task*>::Iterator* iterator = plan.iterator();
    while (iterator->hasNext()) {
        AMW_Task *nextTask = iterator->next();
        if (currentIndex > 0) {
            float distance =
                    (position - pickupLocation).length() +
                    (deliveryLocation - nextTask->getStartPosition(deliveryLocation)).length() -
                    (position - nextTask->getStartPosition(position)).length();
            if (minDistance == -1 || distance <= minDistance) {
                minDistance = distance;
                bestPosition = currentIndex;
            }
        }
        currentIndex++;
        Vector3f nextEnd = nextTask->getEndPosition(position);
        if (nextEnd != NULL)
            position = nextEnd;
    }

    Vector3f finalTask = plan.tail;
    float distance = (position - pickupLocation).length();
    if (minDistance == -1 || distance <= minDistance) {
        minDistance = distance;
        bestPosition = currentIndex;
    }

    if (!estimate)
        plan.insert(bestPosition, package);

    return minDistance;
}
