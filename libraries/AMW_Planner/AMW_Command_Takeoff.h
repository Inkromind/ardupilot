/*
 * AMW_Command_Takeoff.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_TAKEOFF_H_
#define AMW_COMMAND_TAKEOFF_H_

#include "AMW_Command.h"

/**
 * Takeoff to a given altitude
 */
class AMW_Command_Takeoff: public AMW_Command {
public:
    /**
     * @param alt The altitude to take off to (in cm)
     */
    AMW_Command_Takeoff(float alt);

    virtual void run(bool attempt);

protected:
    float altitude;
    virtual void updateStatus();
};

#endif /* AMW_TASK_TAKEOFF_H_ */
