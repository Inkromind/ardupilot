/*
 * AMW_Task.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

class AMW_Task {
public:
    virtual ~AMW_Task();

    virtual void updateStatus(void) = 0;

    virtual void runTask(void) = 0;

    virtual bool isComplete(void) {
        return completed;
    }

private:
    bool running = false;
    bool completed = false;
};

#endif /* AMW_TASK_H_ */
