/*
 * AMW_Corridor_Conflict.cpp
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#include "AMW_Corridor_Conflict.h"

AMW_Corridor_Conflict::AMW_Corridor_Conflict(AMW_Corridor* ownCorridor, AMW_Corridor* otherCorridor) {
    ownType = ownCorridor->getType();
    ownId = ownCorridor->getId();
    otherType = otherCorridor->getType();
    otherId = otherCorridor->getId();
    ownAltitude = ownCorridor->getAltitude();
    otherAltitude = otherCorridor->getAltitude();
}

