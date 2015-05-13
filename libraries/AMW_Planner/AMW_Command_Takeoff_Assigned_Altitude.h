/*
 * AMW_Command_Takeoff_Assigned_Altitude.h
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_TAKEOFF_ASSIGNED_ALTITUDE_H_
#define AMW_COMMAND_TAKEOFF_ASSIGNED_ALTITUDE_H_

#include "AMW_Command.h"

class AMW_Command_Takeoff_Assigned_Altitude: public AMW_Command_Takeoff {
public:
    AMW_Command_Takeoff_Assigned_Altitude() : AMW_Command_Takeoff(1500) {};

    void runCommand();
private:
    void updateStatus();
};

#endif /* AMW_COMMAND_TAKEOFF_ASSIGNED_ALTITUDE_H_ */
