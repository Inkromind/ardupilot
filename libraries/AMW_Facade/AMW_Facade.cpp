/*
 * AMW_Facade.cpp
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#include "AMW_Facade.h"
#include <AMW_Planner.h>

void AMW_Facade::initPlanner() {
    AMW_Planner::initPlanner();
}

void AMW_Facade::run100Hz() {
    AMW_Planner::run100Hz();
}

void AMW_Facade::run50Hz() {
    AMW_Planner::run50Hz();
}

void AMW_Facade::run10Hz() {
    AMW_Planner::run10Hz();
}

void AMW_Facade::run1Hz() {
    AMW_Planner::run1Hz();
}

void AMW_Facade::resumeMission() {
    AMW_Planner::resumeMission();
}

void AMW_Facade::pauseMission() {
    AMW_Planner::pauseMission();
}

void AMW_Facade::addPackage(uint8_t id, Vector3f pickupLocation, Vector3f deliveryLocation) {
    AMW_Task_Planner::getInstance()->addPackage(new AMW_Task_Package(id, pickupLocation, deliveryLocation));
}
