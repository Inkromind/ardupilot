/*
 * AMW_Command_Nav_Altitude.h
 *
 *  Created on: 15-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_NAV_ALTITUDE_H_
#define AMW_COMMAND_NAV_ALTITUDE_H_

#include "AMW_Command.h"

class AMW_Command_Nav_Altitude: public AMW_Command {
public:
    AMW_Command_Nav_Altitude(float alt);

    virtual void run(bool attempt);

private:
    float altitude;
    void updateStatus();
};

#endif /* AMW_COMMAND_NAV_ALTITUDE_H_ */
