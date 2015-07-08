/*
 * AMW_Task_Package.h
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_PACKAGE_H_
#define AMW_TASK_PACKAGE_H_

#define PACKAGE_WAIT_TIME 15;

#include "AMW_Task.h"
#include "../AP_Math/vector2.h"

class AMW_Task_Package: public AMW_Task {
public:
    AMW_Task_Package(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation);

    AMW_Command* generatePlan(void);

    Vector2f getEndPosition(Vector2f currentPosition) {
        return deliveryLocation;
    }

    Vector2f getStartPosition(Vector2f currentPosition) {
        return pickupLocation;
    }

    void completeTask(void);

    Vector2f pickupLocation;
    Vector2f deliveryLocation;
    uint8_t id;
};

#endif /* AMW_TASK_PACKAGE_H_ */
