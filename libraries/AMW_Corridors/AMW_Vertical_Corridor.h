/*
 * AMW_Vertical_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_VERTICAL_CORRIDOR_H_
#define AMW_VERTICAL_CORRIDOR_H_

#include "AMW_Directional_Corridor.h"
#include "../AP_Math/vector2.h"

class AMW_Vertical_Corridor: public AMW_Directional_Corridor {
public:
    AMW_Vertical_Corridor(Vector2f newLocation);
    AMW_Vertical_Corridor(Vector2f newLocation, float newAltitude);
    AMW_Vertical_Corridor(Vector2f newLocation, float newAltitude, uint8_t newId);
    virtual ~AMW_Vertical_Corridor() {
    }

    virtual AMW_Corridor::Type getType(void) const {
        return VERTICAL;
    }

protected:
    virtual Vector3f getStart(void) const;
    virtual Vector3f getDestination(void) const;

private:
    Vector2f location;
};

#endif /* LIBRARIES_AMW_CORRIDORS_AMW_VERTICAL_CORRIDOR_H_ */
