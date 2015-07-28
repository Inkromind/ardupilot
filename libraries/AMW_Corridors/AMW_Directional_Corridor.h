/*
 * AMW_Directional_Corridor.h
 *
 *  Created on: 20-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_DIRECTIONAL_CORRIDOR_H_
#define AMW_DIRECTIONAL_CORRIDOR_H_

#include "AMW_Corridor.h"
#include "../AP_Math/vector3.h"
#include <AC_Facade.h>

class AMW_Directional_Corridor: public AMW_Corridor {
public:
    virtual ~AMW_Directional_Corridor() {
    }

    virtual bool checkForConflicts(bool checkFullCorridor) const {
        return (!checkFullCorridor || !completed);
    }

    virtual void setReverseDirection(bool newValue) {
        reverseDirection = newValue;
    }

    virtual void setInCorridor(bool newValue) {
        inCorridor = newValue;
    }

    virtual void setCompleted(bool newValue) {
        completed = newValue;
    }

    virtual Vector3f getStartPoint(bool checkFullCorridor) const {
        if (checkFullCorridor || !inCorridor) {
            if (!reverseDirection)
                return getStart();
            else
                return getDestination();
        }
        else {
            return AC_Facade::getFacade()->getRealPosition();
        }
    }

    virtual Vector3f getEndPoint(bool checkFullCorridor) const {
        if (!reverseDirection)
            return getDestination();
        else
            return getStart();
    }


protected:
    bool reverseDirection;
    bool inCorridor;
    bool completed;

    AMW_Directional_Corridor() : AMW_Corridor() {
        reverseDirection = false;
        completed = false;
        inCorridor = false;
    };

    AMW_Directional_Corridor(uint8_t id) : AMW_Corridor(id) {
        reverseDirection = false;
        completed = false;
        inCorridor = false;
    };

    virtual Vector3f getStart(void) const = 0;
    virtual Vector3f getDestination(void) const = 0;
};

#endif /* AMW_DIRECTIONAL_CORRIDOR_H_ */
