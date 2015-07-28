/*
 * AMW_Vertical_Corridor.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Vertical_Corridor.h"

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f location) : AMW_Directional_Corridor() {
    this->location = location;
}

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f location, float altitude) {
    this->location = location;
    this->altitude = altitude;
}

AMW_Vertical_Corridor::AMW_Vertical_Corridor(Vector2f location, float altitude, uint8_t id) : AMW_Directional_Corridor(id) {
    this->altitude = altitude;
    this->location = location;
}

Vector3f AMW_Vertical_Corridor::getStart(void) {
    return Vector3f(location.x, location.y, 0);
}

Vector3f AMW_Vertical_Corridor::getDestination(void) {
    return Vector3f(location.x, location.y, altitude);
}

