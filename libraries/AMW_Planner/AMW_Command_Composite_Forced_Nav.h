/*
 * AMW_Command_Composite_Nav_Assigned_Altitude_Forced.h
 *
 *  Created on: 2-aug.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_FORCED_H_
#define AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_FORCED_H_

#include "AMW_Command_Composite_Nav.h"
#define AMW_COMMAND_COMPOSITE_NAV_FORCED_DESTINATION_RADIUS 200.0f

/**
 * Do a forced travel and do not wait for the reservation of corridors
 */
class AMW_Command_Composite_Forced_Nav: public AMW_Command_Composite_Nav {
public:
    AMW_Command_Composite_Forced_Nav(Vector2f newDestination, bool returnHome = false) :
        AMW_Command_Composite_Nav(newDestination, returnHome) {};
    virtual void startCommand(bool attempt);

    virtual void updateStatus(void);
};

#endif /* AMW_COMMAND_COMPOSITE_NAV_ASSIGNED_ALTITUDE_FORCED_H_ */
