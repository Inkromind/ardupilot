/*
 * AMW_Logging.cpp
 *
 *  Created on: 29-nov.-2015
 *      Author: Arne
 */

#include <AMW_Logging.h>

AMW_Logging* AMW_Logging::getInstance() {
    return 0;
}

void AMW_Logging::sync(uint8_t id) {}

void AMW_Logging::logCompletedReservation(float level, uint8_t attempts) {}

void AMW_Logging::logFailedReservation(uint8_t attempts) {}

void AMW_Logging::logPosition(void) {}

void AMW_Logging::logConflict(void) {}

void AMW_Logging::logAssignedPackage(uint8_t packageId, float estimate) {}

void AMW_Logging::logCompletedPackage(uint8_t packageId) {}

void AMW_Logging::logFailedPackage(uint8_t packageId) {}

