/*
 * AMW_Corridor_Conflict.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_CONFLICT_H_
#define AMW_CORRIDOR_CONFLICT_H_

#include <stdint.h>

/**
 * Conflict between two corridors
 */
class AMW_Corridor_Conflict {
public:
    AMW_Corridor_Conflict(uint8_t ownType, uint8_t ownId, float ownAltitude,
            uint8_t otherType, uint8_t otherId, float otherAltitude);
    virtual ~AMW_Corridor_Conflict() {
    }

    /**
     * Get the Type of our corridor that was involved in the conflict
     */
    virtual uint8_t getOwnType(void) const {
        return ownType;
    }

    /**
     * Get the type of the other corridor that was involved in the conflict
     */
    virtual uint8_t getOtherType(void) const {
        return otherType;
    }

    /**
     * Get the id of our corridor that was involved in the conflict
     */
    virtual uint8_t getOwnId(void) const {
        return ownId;
    }

    /**
     * Get the id of the other corridor that was involved in the conflict
     */
    virtual uint8_t getOtherId(void) const {
        return otherId;
    }

    /**
     * Get the altitude of our corridor that was involved in the conflict
     */
    virtual float getOwnAltitude(void) const {
        return ownAltitude;
    }

    /**
     * Get the altitude of the other corridor that was involved in the conflict
     */
    virtual float getOtherAltitude(void) const {
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
