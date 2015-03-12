/*
 * AMW_Command_Nav.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_NAV_H_
#define AMW_COMMAND_NAV_H_

#include <AMW_Command.h>

class AMW_Command_Nav: public AMW_Command {
public:
    AMW_Command_Nav(Vector3f destination);
    virtual ~AMW_Command_Nav(void) {
        delete destination;
    }

    void updateStatus(void);
    void runCommand(void);

private:
    Vector3f destination;
};

#endif /* AMW_COMMAND_NAV_H_ */
