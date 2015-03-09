/*
 * AMW_Task_Command.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_COMMAND_H_
#define AMW_TASK_COMMAND_H_

class AMW_Task_Command {
public:
    virtual ~AMW_Task_Command();
    virtual void updateStatus(void) = 0;

    virtual void runCommand(void) = 0;

    virtual bool isComplete(void);

private:
    bool running = false;
    bool completed = false;
};

#endif /* AMW_TASK_COMMAND_H_ */
