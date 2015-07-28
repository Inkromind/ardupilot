/*
 * AMW_Horizontal_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_HORIZONTAL_CORRIDOR_H_
#define AMW_HORIZONTAL_CORRIDOR_H_

#include "AMW_Directional_Corridor.h"
#include "../AP_Math/vector2.h"

class AMW_Horizontal_Corridor: public AMW_Directional_Corridor {
public:
    AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f destination);
    AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f destination, float altitude, uint8_t id);
    virtual ~AMW_Horizontal_Corridor() {
    }

    virtual AMW_Corridor::Type getType(void) const {
        return HORIZONTAL;
    }

    virtual Vector3f getStart(void) const;
    virtual Vector3f getDestination(void) const;

private:
    Vector2f start;
    Vector2f destination;
};

#endif /* AMW_HORIZONTAL_CORRIDOR_H_ */
