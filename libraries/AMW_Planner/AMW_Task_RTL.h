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
#include "../AP_Math/vector3.h"

/**
 * Return to the homebase and land
 */
class AMW_Task_RTL: public AMW_Task {
public:
    AMW_Task_RTL();

    AMW_Command* generatePlan(void) const;

    Vector2f getEndPosition(Vector3f currentPosition) const {
        return AMW_Task_Planner::getInstance()->getHomeBase();
    }

    Vector2f getStartPosition(Vector3f currentPosition) const {
        return getEndPosition(currentPosition);
    }

    void completeTask(void);
};

#endif /* AMW_TASK_RTL_H_ */
