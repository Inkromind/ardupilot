/*
 * AMW_Command_Nav_Assigned_Altitude.h
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_
#define AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_

#include "AMW_Command_Nav.h"
#include "../AP_Math/vector3.h"
#include "../AP_Math/vector2.h"

class AMW_Command_Nav_Assigned_Altitude : public AMW_Command_Nav {
public:
    AMW_Command_Nav_Assigned_Altitude(Vector2f newDestination) :
        AMW_Command_Nav(Vector3f(newDestination.x, newDestination.y, 0)) {};

    void run(void);

private:
    void updateStatus(void);
};

#endif /* AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_ */
