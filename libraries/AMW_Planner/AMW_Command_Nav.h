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

/**
 * Fly to a given location
 */
class AMW_Command_Nav: public AMW_Command {
public:
    /**
     * @param destination The location to fly to (relative to origin)
     */
    AMW_Command_Nav(Vector3f destination);

    virtual void run(bool attempt);

protected:
    Vector3f destination;
private:
    virtual void updateStatus(void);
};

#endif /* AMW_COMMAND_NAV_H_ */
