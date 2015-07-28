/*
 * AMW_Position_Corridor.h
 *
 *  Created on: 18-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_POSITION_CORRIDOR_H_
#define AMW_POSITION_CORRIDOR_H_

#include "AMW_Corridor.h"
#include "../AP_Math/vector3.h"
#include "../AP_Math/vector2.h"

class AMW_Position_Corridor: public AMW_Corridor {
public:
    AMW_Position_Corridor(Vector3f position);
    AMW_Position_Corridor(Vector3f position, uint8_t id);
    virtual ~AMW_Position_Corridor() {}

    virtual AMW_Corridor::Type getType(void) {
        return POSITION;
    }

    virtual Vector3f getStartPoint(bool checkFullCorridor) {
        return Vector3f(location.x, location.y, altitude);
    }
    virtual Vector3f getEndPoint(bool checkFullCorridor ) {
        return Vector3f(location.x, location.y, altitude);
    }

    virtual bool checkForConflicts(bool checkFullCorridor) {
        return true;
    }


private:
    Vector2f location;
};

#endif /* AMW_POSITION_CORRIDOR_H_ */
