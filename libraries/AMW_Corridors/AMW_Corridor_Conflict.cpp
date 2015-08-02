/*
 * AMW_Corridor_Conflict.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Conflict.h"

AMW_Corridor_Conflict::AMW_Corridor_Conflict(uint8_t newOwnType, uint8_t newOwnId, float newOwnAltitude,
            uint8_t newOtherType, uint8_t newOtherId, float newOtherAltitude) {
    this->ownType = newOwnType;
    this->ownId = newOwnId;
    this->otherType = newOtherType;
    this->otherId = newOtherId;
    this->ownAltitude = newOwnAltitude;
    this->otherAltitude = newOtherAltitude;
}

