/*
 * AMW_Task_Land.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_LAND_H_
#define AMW_TASK_LAND_H_

#include "AMW_Planner_Task.h"

class AMW_Task_Land: public AMW_Planner_Task {
public:
    AMW_Task_Land(void);

    void updateStatus(void);
    void runTask(void);
};

#endif /* AMW_TASK_LAND_H_ */
