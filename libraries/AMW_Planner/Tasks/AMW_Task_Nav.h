/*
 * AMW_Task_Nav.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_NAV_H_
#define AMW_TASK_NAV_H_

class AMW_Task_Nav: public AMW_Planner_Task {
public:
    AMW_Task_Nav(Vector3f destination);
    virtual ~AMW_Task_Nav();

    void updateStatus();
    void runTask();
private:
    Vector3f destination;
};

#endif /* AMW_TASK_NAV_H_ */
