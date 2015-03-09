/*
 * AMW_Task_Land.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_LAND_H_
#define AMW_TASK_LAND_H_

class AMW_Task_Land: public AMW_Planner_Task {
public:
    AMW_Task_Land();
    virtual ~AMW_Task_Land();

    void updateStatus();
    void runTask();
};

#endif /* AMW_TASK_LAND_H_ */
