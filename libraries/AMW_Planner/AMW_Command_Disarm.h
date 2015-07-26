/*
 * AMW_Command_Disarm.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_DISARM_H_
#define AMW_COMMAND_DISARM_H_

#include "AMW_Command.h"

class AMW_Command_Disarm: public AMW_Command {
public:
    AMW_Command_Disarm();

    void run(bool attempt);
    void run();

private:
    void updateStatus(void);

};

#endif /* AMW_COMMAND_DISARM_H_ */
