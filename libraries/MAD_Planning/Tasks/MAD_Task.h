/*
 * MAD_Task.h
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_H_
#define MAD_TASK_H_

class MAD_Task {

public:
    virtual ~MAD_Task();

    virtual bool isComplete() = 0;

    virtual void runTask() = 0;

private:
    bool running = false;
    bool completed = false;
};

#endif /* MADTASK_H_ */
