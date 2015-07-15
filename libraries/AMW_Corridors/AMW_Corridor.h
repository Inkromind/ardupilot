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
    virtual void setAltitude(float altitude) = 0;
    virtual ~AMW_Corridor() { }
    virtual float getAltitude(void) = 0;
    virtual AMW_Corridor_Conflict* checkConflicts(AMW_List<AMW_Corridor*>* corridors, bool checkFullCorridor);

};



#endif /* AMW_CORRIDOR_H_ */
