/*
 * AMW_Commands_Plan.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMANDS_PLAN_H_
#define AMW_COMMANDS_PLAN_H_

#include "AMW_Command.h"
#include <AMW_Queue.h>

class AMW_Commands_Plan {
public:
    AMW_Commands_Plan();
    virtual ~AMW_Commands_Plan();

    virtual void executePlan(void);
    virtual bool isCompleted(void);
    virtual bool hasFailed(void);

    virtual void addNewCommand(AMW_Command* command);

    virtual void pausePlan(void);
    virtual void resumePlan(void);

private:
    bool completed;
    bool failed;
    bool paused;

    AMW_Queue<AMW_Command*> plan;
    AMW_Command* currentCommand;

    virtual void completeCurrentCommand(void);
};

#endif /* AMW_COMMANDS_PLAN_H_ */
