/*
 * AMW_Command_Nav_Assigned_Altitude.h
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_
#define AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_

#include "AMW_Command.h"
#include <AP_Math.h>

class AMW_Command_Nav_Assigned_Altitude: public AMW_Command_Nav {
public:
    virtual AMW_Command_Nav_Assigned_Altitude(Vector2f destination) :
        AMW_Command_Nav(Vector3f(destination.x, destination.y, 0)) {};

    void runCommand(void);

private:
    void updateStatus(void);
};

#endif /* AMW_COMMAND_NAV_ASSIGNED_ALTITUDE_H_ */
