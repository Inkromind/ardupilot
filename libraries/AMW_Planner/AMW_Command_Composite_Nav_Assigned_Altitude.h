/*
 * AMW_Command_Composite_Nav_Assigned_Altitude.h
 *
 *  Created on: 21-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_
#define AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_

#include "AMW_Command_Composite.h"
#include "../AP_Math/vector2.h"
#include "../AP_Math/vector3.h"

class AMW_Command_Composite_Nav_Assigned_Altitude: public AMW_Command_Composite {
public:
    AMW_Command_Composite_Nav_Assigned_Altitude(Vector2f destination);
    virtual ~AMW_Command_Composite_Nav_Assigned_Altitude() {}

    virtual void updateStatus(void);

private:
    Vector3f destination;
};

#endif /* AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_H_ */
