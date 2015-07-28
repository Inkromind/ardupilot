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

void AMW_Facade::addPackage(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation, bool estimate) {
    AMW_Task_Planner::getInstance()->addPackage(new AMW_Task_Package(id, pickupLocation, deliveryLocation), estimate);
}

AMW_Corridor_Conflict* AMW_Facade::checkReservationRequest(AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->checkReservationRequest(corridors);
}

void AMW_Facade::reservationConflictReceived(uint8_t reservationId, AMW_Corridor_Conflict* conflict) {
    AMW_Corridor_Manager::getInstance()->reservationConflictReceived(reservationId, conflict);
}

void AMW_Facade::receivedCorridorBroadcast(AMW_List<AMW_Corridor*>* corridors) {
    AMW_Corridor_Manager::getInstance()->receivedCorridorBroadcast(corridors);
}

bool AMW_Facade::markCorridorsReserved(AMW_Module_Identifier* module, AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->markCorridorsReserved(module, corridors);
}

void AMW_Facade::freeCorridors(AMW_List<AMW_Corridor*>* corridors) {
    return AMW_Corridor_Manager::getInstance()->freeCorridors(corridors);
}
