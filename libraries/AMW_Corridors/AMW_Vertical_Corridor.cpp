/*
 * AMW_Vertical_Corridor.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Vertical_Corridor.h"

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f newLocation) : AMW_Directional_Corridor() {
    this->location = newLocation;
}

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f newLocation, float newAltitude) {
    this->location = newLocation;
    this->altitude = newAltitude;
}

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f newLocation, float newAltitude, uint8_t newId) : AMW_Directional_Corridor(newId) {
    this->altitude = newAltitude;
    this->location = newLocation;
}

Vector3f AMW_Vertical_Corridor::getStart(void) const {
    return Vector3f(location.x, location.y, 0);
}

Vector3f AMW_Vertical_Corridor::getDestination(void) const {
    return Vector3f(location.x, location.y, altitude);
}

