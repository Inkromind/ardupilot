/*
 * AMW_Horizontal_Corridor.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Horizontal_Corridor.h"

AMW_Horizontal_Corridor::AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f newDestination) : AMW_Directional_Corridor() {
    start = startLocation;
    this->destination = newDestination;
}

AMW_Horizontal_Corridor::AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f newDestination, float newAltitude, uint8_t newId) : AMW_Directional_Corridor(newId) {
    start = startLocation;
    this->destination = newDestination;
    this->altitude = newAltitude;
}

Vector3f AMW_Horizontal_Corridor::getStart(void) const {
    return Vector3f(start.x, start.y, altitude);
}
Vector3f AMW_Horizontal_Corridor::getDestination(void) const {
    return Vector3f(destination.x, destination.y, altitude);
}
