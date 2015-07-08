/*
 * AMW_Facade.h
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_FACADE_H_
#define AMW_FACADE_H_

#include "../AP_Math/vector2.h"
#include <stdint.h>

class AMW_Facade {

public:
    static void initPlanner();

    static void resumeMission();

    static void pauseMission();

    static void returnHome();

    static void addPackage(uint8_t id, Vector2f pickupLocation, Vector2f deliveryLocation);

    static void run100Hz();

    static void run50Hz();

    static void run10Hz();

    static void run1Hz();

private:
    AMW_Facade();
    ~AMW_Facade();
};

#endif /* AMW_FACADE_H_ */
