/*
 * AMW_Task_Takeoff.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_TAKEOFF_H_
#define AMW_TASK_TAKEOFF_H_

class AMW_Task_Takeoff: public AMW_Planner_Task {
public:
    AMW_Task_Takeoff(float alt);
    virtual ~AMW_Task_Takeoff();

    void updateStatus();
    void runTask();

private:
    float altitude;
};

#endif /* AMW_TASK_TAKEOFF_H_ */
