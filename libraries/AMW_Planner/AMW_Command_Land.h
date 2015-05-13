/*
 * AMW_Command_Land.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_LAND_H_
#define AMW_COMMAND_LAND_H_

#include "AMW_Command.h"

class AMW_Command_Land : public AMW_Command {
public:
    AMW_Command_Land();

    void runCommand(void);

private:
    void updateStatus(void);
};

#endif /* AMW_COMMAND_LAND_H_ */
