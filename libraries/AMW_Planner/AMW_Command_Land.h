/*
 * AMW_Command_Land.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_LAND_H_
#define AMW_COMMAND_LAND_H_

#include "AMW_Command.h"

/**
 * Land at the current location
 */
class AMW_Command_Land : public AMW_Command {
public:
    AMW_Command_Land();

    void run(bool attempt);

private:
    void updateStatus(void);
};

#endif /* AMW_COMMAND_LAND_H_ */
