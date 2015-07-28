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
#include <AMW_List.h>

#ifndef UINT8_MAX
#define AMW_MAX_CORRIDOR_ID 0xff
#else
#define AMW_MAX_CORRIDOR_ID UINT8_MAX
#endif

#define AMW_MIN_CORRIDOR_DISTANCE 3000

class AMW_Corridor {

public:
    enum Type : uint8_t { VERTICAL=1, POSITION=2, HORIZONTAL=3 };

    virtual void setAltitude(float newAltitude) {
        this->altitude = newAltitude;
    }
    virtual ~AMW_Corridor() { }

    virtual float getAltitude(void) {
        return this->altitude;
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
        return this->id;
    }

    virtual AMW_Corridor::Type getType(void) = 0;

    virtual void setReverseDirection(bool newValue) {}

    virtual void setInCorridor(bool newValue) {}

    virtual void setCompleted(bool newValue) {}

    virtual Vector3f getStartPoint(bool checkFullCorridor = true) = 0;
    virtual Vector3f getEndPoint(bool checkFullCorridor = true) = 0;

protected:
    AMW_Corridor() {
        this->altitude = 0;
        this->id = AMW_Corridor::nextId;
        if (AMW_MAX_CORRIDOR_ID <= AMW_Corridor::nextId)
            AMW_Corridor::nextId = 0;
        else
            AMW_Corridor::nextId++;
    }
    AMW_Corridor(uint8_t id) {
        this->altitude = 0;
        this->id = id;
    }

    float altitude;
    uint8_t id;

    static uint8_t nextId;

    virtual bool checkForConflicts(bool checkFullCorridor = true) = 0;

private:
    AMW_Corridor_Conflict* checkConflict(AMW_Corridor* corridor, bool checkFullCorridor) {
        if (!corridor)
            return 0;
        float distance = getDistance(corridor, checkFullCorridor);
        if (distance < AMW_MIN_CORRIDOR_DISTANCE)
            return new AMW_Corridor_Conflict(getType(), getId(), getAltitude(),
                    corridor->getType(), corridor->getId(), corridor->getAltitude());
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
