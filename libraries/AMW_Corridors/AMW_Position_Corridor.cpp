/*
 * AMW_Position_Corridor.cpp
 *
 *  Created on: 18-jul.-2015
 *      Author: Arne
 */

#include "AMW_Position_Corridor.h"

AMW_Position_Corridor::AMW_Position_Corridor(Vector3f position) : AMW_Corridor() {
    location = Vector2f(position.x, position.y);
    altitude = position.z;
}

AMW_Position_Corridor::AMW_Position_Corridor(Vector3f position, uint8_t id) : AMW_Corridor(id) {
    location = Vector2f(position.x, position.y);
    altitude = position.z;
}

