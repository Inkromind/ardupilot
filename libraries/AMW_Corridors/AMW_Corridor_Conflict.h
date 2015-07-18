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
    AMW_Corridor_Conflict();
    virtual ~AMW_Corridor_Conflict() {
        // TODO Auto-generated destructor stub
    }

    virtual AMW_Corridor::Type getCorridorType(void);
    virtual AMW_Corridor::Type getCorridor2Type(void);
    virtual uint8_t getCorridorId();

    virtual float getAltitude(void);
};

#endif /* AMW_CORRIDOR_CONFLICT_H_ */
