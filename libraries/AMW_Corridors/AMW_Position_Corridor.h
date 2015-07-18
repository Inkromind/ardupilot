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

class AMW_Position_Corridor: public AMW_Corridor {
public:
    AMW_Position_Corridor(Vector3f position);
    virtual ~AMW_Position_Corridor() {}

};

#endif /* AMW_POSITION_CORRIDOR_H_ */
