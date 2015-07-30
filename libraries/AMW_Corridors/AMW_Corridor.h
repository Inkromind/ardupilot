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

/** Radius of corridors */
#define AMW_MIN_CORRIDOR_DISTANCE 300

/**
 * Flight corridor
 */
class AMW_Corridor {

public:
    enum Type : uint8_t { VERTICAL=1, POSITION=2, HORIZONTAL=3 };

    /**
     * Set the altitude of this corridor
     */
    virtual void setAltitude(float newAltitude) {
        this->altitude = newAltitude;
    }
    virtual ~AMW_Corridor() { }

    /**
     * Get the altitude of this corridor
     */
    virtual float getAltitude(void) const {
        return this->altitude;
    }

    /**
     * Check if any of the given corridors conflict with this corridor
     *
     * @param corridors List of corridors to check
     * @param checkFullCorridor Set to false to only check the part of this
     * corridor that has not yet been finished
     * @return The first conflict found, or 0 if no conflicts
     */
    virtual AMW_Corridor_Conflict* checkConflicts(const AMW_List<AMW_Corridor*>* corridors, bool checkFullCorridor) const {
        if (!corridors)
            return 0;

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

    /**
     * Get the id of this corridor
     */
    virtual uint8_t getId() const {
        return this->id;
    }

    /**
     * Get the type of this corridor
     */
    virtual AMW_Corridor::Type getType(void) const = 0;

    /**
     * Set if this corridor is being travelled along in the opposite direction
     */
    virtual void setReverseDirection(bool newValue) {}

    /**
     * Set if currently in this corridor
     */
    virtual void setInCorridor(bool newValue) {}

    /**
     * Set if currently in this corridor
     */
    virtual void setCompleted(bool newValue) {}

    /**
     * Get the starting point of this corridor
     *
     * @param checkFullCorridor set to false to get the starting point of the
     * part not yet completed
     */
    virtual Vector3f getStartPoint(bool checkFullCorridor = true) const = 0;

    /**
     * Get the destination of this corridor
     */
    virtual Vector3f getEndPoint() const = 0;

    static uint8_t nextId;
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

    /**
     * Check if conflicts for this corridor need to be computed
     */
    virtual bool checkForConflicts(bool checkFullCorridor = true) const = 0;

private:
    /**
     * Check if the given corridor conflicts with this corridor
     */
    AMW_Corridor_Conflict* checkConflict(const AMW_Corridor* corridor, bool checkFullCorridor) const {
        if (!corridor)
            return 0;
        float distance = getDistance(corridor, checkFullCorridor);
        if (distance < AMW_MIN_CORRIDOR_DISTANCE)
            return new AMW_Corridor_Conflict(getType(), getId(), getAltitude(),
                    corridor->getType(), corridor->getId(), corridor->getAltitude());
        else
            return 0;
    }

    /**
     * Get the closest distance to the given corridor
     */
    float getDistance(const AMW_Corridor* corridor, bool checkFullCorridor) const {
        if (getType() == POSITION || corridor->getType() == POSITION) {
            if (getType() == POSITION && corridor->getType() == POSITION) {
                return dist_Point_to_Point(getStartPoint(), corridor->getStartPoint());
            }
            else if (getType() == POSITION) {
                return dist_Point_to_Segment(getStartPoint(), corridor->getStartPoint(), corridor->getEndPoint());
            }
            else {
                return dist_Point_to_Segment(corridor->getStartPoint(), getStartPoint(checkFullCorridor), getEndPoint());
            }
        }
        else {
            return dist3D_Segment_to_Segment(getStartPoint(checkFullCorridor), getEndPoint(), corridor->getStartPoint(), corridor->getEndPoint());
        }
    }

};

#endif /* AMW_CORRIDOR_H_ */
