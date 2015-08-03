/*
 * AMW_Facade.cpp
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#include "AMW_Facade.h"
#include <AMW_Planner.h>
#include <AMW_Corridors.h>

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
    AMW_Corridor_Manager::getInstance()->broadcastReservedCorridors();
    AMW_Corridor_Manager::getInstance()->checkTimeout();
}

void AMW_Facade::resumeMission() {
    AMW_Planner::resumeMission();
}

void AMW_Facade::pauseMission() {
    AMW_Planner::pauseMission();
}

void AMW_Facade::returnHome() {
    AMW_Planner::returnHome();
}

float AMW_Facade::addPackage(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation, bool estimate) {
    return AMW_Task_Planner::getInstance()->addTask(new AMW_Task_Package(id, pickupLocation, deliveryLocation), estimate);
}

AMW_Corridor_Conflict* AMW_Facade::checkReservationRequest(const AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->checkReservationRequest(corridors);
}

void AMW_Facade::reservationConflictReceived(uint8_t reservationId, const AMW_Corridor_Conflict* conflict) {
    AMW_Corridor_Manager::getInstance()->reservationConflictReceived(reservationId, conflict);
}

void AMW_Facade::receivedCorridorBroadcast(const AMW_List<AMW_Corridor*>* corridors) {
    AMW_Corridor_Manager::getInstance()->receivedCorridorBroadcast(corridors);
}

bool AMW_Facade::markCorridorsReserved(const AMW_Module_Identifier* module, const AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->markCorridorsReserved(module, corridors);
}

void AMW_Facade::freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->freeCorridors(corridors);
}

bool AMW_Facade::isBatteryEmpty() {
    return AMW_Planner::isBatteryEmpty();
}
void AMW_Facade::markBatteryEmpty() {
    AMW_Planner::markBatteryEmpty();
}


void AMW_Facade::setHomebase(Vector2f newHomebase) {
    AMW_Planner::setHomebase(newHomebase);
}
