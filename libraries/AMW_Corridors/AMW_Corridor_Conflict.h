/*
 * AMW_Corridor_Conflict.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_CONFLICT_H_
#define AMW_CORRIDOR_CONFLICT_H_

#include "AMW_Corridor.h"
#include <stdint.h>

class AMW_Corridor_Conflict {
public:
    AMW_Corridor_Conflict(AMW_Corridor* ownCorridor, AMW_Corridor* otherCorridor);
    AMW_Corridor_Conflict(AMW_Corridor::Type ownType, uint8_t ownId, float ownAltitude,
            AMW_Corridor::Type otherType, uint8_t otherId, float otherAltitude);
    virtual ~AMW_Corridor_Conflict() {
    }

    virtual AMW_Corridor::Type getOwnType(void) {
        return ownType;
    }
    virtual AMW_Corridor::Type getOtherType(void) {
        return otherType;
    }
    virtual uint8_t getOwnId(void) {
        return ownId;
    }
    virtual uint8_t getOtherId(void) {
        return otherId;
    }

    virtual float getOwnAltitude(void) {
        return ownAltitude;
    }
    virtual float getOtherAltitude(void) {
        return otherAltitude;
    }

private:
    AMW_Corridor::Type ownType;
    AMW_Corridor::Type otherType;
    uint8_t ownId;
    uint8_t otherId;
    float ownAltitude;
    float otherAltitude;
};

#endif /* AMW_CORRIDOR_CONFLICT_H_ */
