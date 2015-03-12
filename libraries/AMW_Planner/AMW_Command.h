/*
 * AMW_Task.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

class AMW_Command {
public:
    virtual ~AMW_Command();

    virtual void updateStatus(void) = 0;

    virtual void runCommand(void) = 0;

    virtual bool isComplete(void) {
        return completed;
    }

private:
    bool running = false;
    bool completed = false;
};

#endif /* AMW_TASK_H_ */
