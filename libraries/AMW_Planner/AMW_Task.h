/*
 * AMW_Task.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

#include "AMW_Commands_Plan.h"
#include <AP_Math.h>

class AMW_Task {
public:
    virtual ~AMW_Task() {};

    virtual AMW_Commands_Plan* generatePlan(void) = 0;

    virtual Vector2f getStartPosition(Vector2f currentPosition) {
        return currentPosition;
    }

    virtual Vector2f getEndPosition(Vector2f currentPosition) {
        return currentPosition;
    }
};

#endif /* AMW_TASK_H_ */
