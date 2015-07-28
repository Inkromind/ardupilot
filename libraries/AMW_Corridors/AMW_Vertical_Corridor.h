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
    AMW_Vertical_Corridor(Vector2f location);
    AMW_Vertical_Corridor(Vector2f location, float altitude);
    AMW_Vertical_Corridor(Vector2f location, float altitude, uint8_t id);
    virtual ~AMW_Vertical_Corridor() {
    }

    virtual AMW_Corridor::Type getType(void) const {
        return VERTICAL;
    }

    virtual Vector3f getStart(void) const;
    virtual Vector3f getDestination(void) const;

private:
    Vector2f location;
};

#endif /* LIBRARIES_AMW_CORRIDORS_AMW_VERTICAL_CORRIDOR_H_ */
