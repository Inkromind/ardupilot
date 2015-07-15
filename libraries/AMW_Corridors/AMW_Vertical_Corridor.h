/*
 * AMW_Vertical_Corridor.h
 *
 *  Created on: 15-jul.-2015
 *      Author: Arne
 */

#ifndef AMW_VERTICAL_CORRIDOR_H_
#define AMW_VERTICAL_CORRIDOR_H_

#include "AMW_Corridor.h"
#include "../AP_Math/vector2.h"
#include <stdint.h>

class AMW_Vertical_Corridor: public AMW_Corridor {
public:
    AMW_Vertical_Corridor(Vector2f location);
    AMW_Vertical_Corridor(Vector2f location, float altitude);
    virtual ~AMW_Vertical_Corridor() {
        // TODO Auto-generated destructor stub
    }
};

#endif /* LIBRARIES_AMW_CORRIDORS_AMW_VERTICAL_CORRIDOR_H_ */
