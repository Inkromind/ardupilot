/*
 * AMW_Corridor_Conflict.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_CONFLICT_H_
#define AMW_CORRIDOR_CONFLICT_H_

#include <stdint.h>

class AMW_Corridor_Conflict {
public:
    AMW_Corridor_Conflict(uint8_t ownType, uint8_t ownId, float ownAltitude,
            uint8_t otherType, uint8_t otherId, float otherAltitude);
    virtual ~AMW_Corridor_Conflict() {
    }

    virtual uint8_t getOwnType(void) {
        return ownType;
    }
    virtual uint8_t getOtherType(void) {
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
    uint8_t ownType;
    uint8_t otherType;
    uint8_t ownId;
    uint8_t otherId;
    float ownAltitude;
    float otherAltitude;
};

#endif /* AMW_CORRIDOR_CONFLICT_H_ */
