/*
 * AMW_Task_RTL.h
 *
 *  Created on: 18-mei-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_RTL_H_
#define AMW_TASK_RTL_H_

#include "AMW_Task.h"
#include "AMW_Task_Planner.h"

class AMW_Task_RTL: public AMW_Task {
public:
    AMW_Task_RTL();

    AMW_Commands_Plan* generatePlan(void);

    Vector2f getEndPosition(Vector3f currentPosition) {
        return AMW_Task_Planner::getInstance()->getHomeBase();
    }

    Vector2f getStartPosition(Vector3f currentPosition) {
        return getEndPosition(currentPosition);
    }
};

#endif /* AMW_TASK_RTL_H_ */
