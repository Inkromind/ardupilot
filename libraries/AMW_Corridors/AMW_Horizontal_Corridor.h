/*
 * AMW_Horizontal_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_HORIZONTAL_CORRIDOR_H_
#define AMW_HORIZONTAL_CORRIDOR_H_

#include "AMW_Corridor.h"
#include "../AP_Math/vector2.h"

class AMW_Horizontal_Corridor: public AMW_Corridor {
public:
    AMW_Horizontal_Corridor(Vector2f startLocation, Vector2f destination);
    virtual ~AMW_Horizontal_Corridor() {
        // TODO Auto-generated destructor stub
    }
};

#endif /* AMW_HORIZONTAL_CORRIDOR_H_ */
