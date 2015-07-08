/*
 * AMW_Command_Nav.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_NAV_H_
#define AMW_COMMAND_NAV_H_

#include "AMW_Command.h"
#include "../AP_Math/vector3.h"

class AMW_Command_Nav: public AMW_Command {
public:
    AMW_Command_Nav(Vector3f destination);

    virtual void run(void);

protected:
    Vector3f destination;
private:
    virtual void updateStatus(void);
};

#endif /* AMW_COMMAND_NAV_H_ */
