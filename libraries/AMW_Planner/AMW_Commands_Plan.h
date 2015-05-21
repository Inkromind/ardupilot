/*
 * AMW_Commands_Plan.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMANDS_PLAN_H_
#define AMW_COMMANDS_PLAN_H_

//#define AMW_COMMANDS_PLAN_DEBUG

#include "AMW_Command_Composite.h"

class AMW_Commands_Plan : public AMW_Command_Composite {
public:
    AMW_Commands_Plan() : AMW_Command_Composite() {}

    virtual ~AMW_Commands_Plan() {}

    void addNewCommand(AMW_Command* command);

    void updateStatus(void) {}
};

#endif /* AMW_COMMANDS_PLAN_H_ */
