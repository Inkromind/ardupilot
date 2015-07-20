/*
 * AMW_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_H_
#define AMW_CORRIDOR_H_

#include <stdint.h>
#include "AMW_Corridor_Conflict.h"
#include "../AP_Math/vector3.h"
#include "SegmentDistance.h"

#ifndef UINT8_MAX
#define AMW_MAX_CORRIDOR_ID 0xff
#else
#define AMW_MAX_CORRIDOR_ID UINT8_MAX
#endif

#define AMW_MIN_CORRIDOR_DISTANCE 3000

class AMW_Corridor {

public:
    enum Type { VERTICAL, POSITION, HORIZONTAL };

    virtual void setAltitude(float newAltitude) {
        altitude = newAltitude;
    }
    virtual ~AMW_Corridor() { }
    virtual float getAltitude(void) {
        return altitude;
    }
    AMW_Corridor_Conflict* checkConflicts(AMW_List<AMW_Corridor*>* corridors, bool checkFullCorridor) {
        if (!checkForConflicts(checkFullCorridor))
            return 0;

        AMW_Corridor_Conflict* conflict = 0;

        AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
        while (iterator->hasNext()) {
            AMW_Corridor* corridor = iterator->next();
            conflict = checkConflict(corridor, checkFullCorridor);
            if (conflict)
                break;
        }
        delete iterator;

        return conflict;
    }
    virtual uint8_t getId() {
        return id;
    }

    virtual Type getType(void) = 0;

protected:
    virtual AMW_Corridor() {
        altitude = 0;
        id = nextId;
        if (AMW_MAX_CORRIDOR_ID <= nextId)
            nextId = 0;
        else
            nextId++;
    }

    float altitude;
    uint8_t id;

    static uint8_t nextId = 0;

    virtual Vector3f getStartPoint(bool checkFullCorridor = true) = 0;
    virtual Vector3f getEndPoint(bool checkFullCorridor = true) = 0;

    virtual bool checkForConflicts(bool checkFullCorridor = true) = 0;

private:
    AMW_Corridor_Conflict* checkConflict(AMW_Corridor* corridor, bool checkFullCorridor) {
        if (!corridor)
            return 0;
        float distance = getDistance(corridor, checkFullCorridor);
        if (distance < AMW_MIN_CORRIDOR_DISTANCE)
            return new AMW_Corridor_Conflict(this, corridor, distance);
        else
            return 0;
    }

    float getDistance(AMW_Corridor* corridor, bool checkFullCorridor) {
        if (getType() == POSITION || corridor->getType() == POSITION) {
            if (getType() == POSITION && corridor->getType() == POSITION) {
                return dist_Point_to_Point(getStartPoint(), corridor->getStartPoint());
            }
            else if (getType() == POSITION) {
                return dist_Point_to_Segment(getStartPoint(), corridor->getStartPoint(), corridor->getEndPoint());
            }
            else {
                return dist_Point_to_Segment(corridor->getStartPoint(), getStartPoint(checkFullCorridor), getEndPoint(checkFullCorridor));
            }
        }
        else {
            return dist3D_Segment_to_Segment(getStartPoint(checkFullCorridor), getEndPoint(checkFullCorridor), corridor->getStartPoint(), corridor->getEndPoint());
        }
    }

};



#endif /* AMW_CORRIDOR_H_ */
