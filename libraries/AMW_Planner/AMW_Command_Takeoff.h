/*
 * AMW_Command_Takeoff.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_TAKEOFF_H_
#define AMW_COMMAND_TAKEOFF_H_

#include "AMW_Command.h"

class AMW_Command_Takeoff: public AMW_Command {
public:
    AMW_Command_Takeoff(float alt);

    virtual void run();

protected:
    float altitude;
    virtual void updateStatus();
};

#endif /* AMW_TASK_TAKEOFF_H_ */
