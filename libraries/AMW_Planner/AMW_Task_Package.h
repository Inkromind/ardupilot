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

/**
 * A task representing a package that has to be picked up at a first location
 * and delivered at another location
 */
class AMW_Task_Package: public AMW_Task {
public:
    /**
     * @param id Package ID
     * @param pickupLocation The location to pick up the package
     * @param deliveryLocation The location to deliver the package to
     */
    AMW_Task_Package(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation);

    AMW_Command* generatePlan(void) const;

    Vector2f getEndPosition() const {
        return deliveryLocation;
    }

    Vector2f getStartPosition() const {
        return pickupLocation;
    }

    void completeTask(void);

    Vector2f pickupLocation;
    Vector2f deliveryLocation;
    uint8_t id;
};

#endif /* AMW_TASK_PACKAGE_H_ */
