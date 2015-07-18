/*
 * AMW_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_CORRIDOR_H_
#define AMW_CORRIDOR_H_

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
    virtual AMW_Corridor_Conflict* checkConflicts(AMW_List<AMW_Corridor*>* corridors, bool checkFullCorridor);
    virtual uint8_t getId();

protected:
    float altitude = 0;

};



#endif /* AMW_CORRIDOR_H_ */
