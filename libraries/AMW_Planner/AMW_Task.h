/*
 * AMW_Task.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

#include "AMW_Command.h"
#include <AP_Math.h>

#define AMW_TASK_DEBUG

class AMW_Task {
public:
    virtual ~AMW_Task() {};

    virtual AMW_Command* generatePlan(void) = 0;

    virtual Vector2f getStartPosition(Vector2f currentPosition) {
        return currentPosition;
    }

    virtual Vector2f getEndPosition(Vector2f currentPosition) {
        return currentPosition;
    }

    virtual void completeTask(void) {
        return;
    }

    int32_t taskId;
};

#endif /* AMW_TASK_H_ */
