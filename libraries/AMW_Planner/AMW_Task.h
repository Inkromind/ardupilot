/*
 * AMW_Task.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_H_
#define AMW_TASK_H_

#include "AMW_Commands_Plan.h"

class AMW_Task {
public:
    virtual ~AMW_Task() {};

    virtual AMW_Commands_Plan* generatePlan(void) = 0;

    virtual Vector3f getStartPosition(Vector3f currentPosition) {
        return NULL;
    }

    virtual Vector3f getEndPosition(Vector3f currentPosition) {
        return NULL;
    }
};

#endif /* AMW_TASK_H_ */
