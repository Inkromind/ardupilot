/*
 * AMW_Task_Disarm.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_DISARM_H_
#define AMW_TASK_DISARM_H_

#include "AMW_Task.h"

class AMW_Task_Disarm: public AMW_Task {
public:
    AMW_Task_Disarm();

    void updateStatus(void);
    void runTask(void);

private:
    Vector3f destination;
};

#endif /* AMW_TASK_DISARM_H_ */
