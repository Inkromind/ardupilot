/*
 * AMW_Horizontal_Corridor.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Horizontal_Corridor.h"

AMW_Horizontal_Corridor::AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f destination) : AMW_Directional_Corridor() {
    start = startLocation;
    this->destination = destination;
}

Vector3f AMW_Horizontal_Corridor::getStart(void) {
    return Vector3f(start.x, start.y, altitude);
}
Vector3f AMW_Horizontal_Corridor::getDestination(void) {
    return Vector3f(destination.x, destination.y, altitude);
}
