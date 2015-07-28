/*
 * AMW_Corridor_Conflict.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Conflict.h"

AMW_Corridor_Conflict::AMW_Corridor_Conflict(uint8_t ownType, uint8_t ownId, float ownAltitude,
            uint8_t otherType, uint8_t otherId, float otherAltitude) {
    this->ownType = ownType;
    this->ownId = ownId;
    this->otherType = otherType;
    this->otherId = otherId;
    this->ownAltitude = ownAltitude;
    this->otherAltitude = otherAltitude;
}

