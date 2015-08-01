/*
 * AMW_Task.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

#include "AMW_Command.h"
#include "../AP_Math/vector2.h"
#include <math.h>

#ifndef TESTENV
    //#define AMW_TASK_DEBUG
#endif

/**
 * A long-term task to complete
 */
class AMW_Task {
public:
    virtual ~AMW_Task() {};

    /**
     * Get the plan of goals to complete to complete this task
     */
    virtual AMW_Command* generatePlan(void) const = 0;

    /**
     * Get the first location this task has to go to
     *
     * @param currentPosition The current position
     */
    virtual Vector2f getStartPosition() const {
        return Vector2f(NAN, NAN);
    }

    /**
     * Get the last location this task has to go to
     *
     * @param currentPosition The current position
     */
    virtual Vector2f getEndPosition() const {
        return Vector2f(NAN, NAN);
    }

    /**
     * Mark this task as completed
     */
    virtual void completedTask(void) {
        return;
    }

    int32_t taskId;
};

#endif /* AMW_TASK_H_ */
