/*
 * AMW_Commands_Plan.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMANDS_PLAN_H_
#define AMW_COMMANDS_PLAN_H_

#include <AMW_Command.h>

class AMW_Commands_Plan {
public:
    AMW_Commands_Plan(AMW_Task& parentTask);
    virtual ~AMW_Commands_Plan();

    virtual void executePlan(void);
    virtual bool isCompleted(void);
    virtual bool hasFailed(void);

    virtual void addNewCommand(AMW_Command* command);

private:
    bool completed = false;
    bool failed = false;

    AMW_Task& parentTask;
    std::queue<AMW_Command*> plan;
    AMW_Command* currentCommand = 0;

    virtual void completeCurrentCommand(void);
};

#endif /* AMW_COMMANDS_PLAN_H_ */
