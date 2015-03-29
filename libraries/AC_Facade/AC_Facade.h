/*
 * AC_Facade.h
 *
 *  Created on: 27-feb.-2015
 *      Author: Arne
 */

#ifndef AC_FACADE_H_
#define AC_FACADE_H_

#include <AP_Math.h>
#include <AP_Progmem.h>

class AC_Facade {

public:
    static bool takeOff(float altitude);
    static bool land(void);
    static bool navigateTo(const Vector3f& destination);
    static bool navigateToAltitude(float altitude);
    static bool armMotors(void);
    static bool disarmMotors(void);
    static bool loiter(void);
    static bool initFlightMode(void);

    static void sendDebug(const prog_char_t *str);

    static float getAltitude(void);
    static bool isLanded(void);
    static bool areMotorsArmed(void);
    static bool destinationReached(const Vector3f& destination);
    static bool altitudeReached(float altitude);
    static Vector3f getPosition(void);

    static uint32_t getCH8Position(void);
    static uint32_t getCH7Position(void);

    //TODO: Remove from AC_Facade
    static AP_BattMonitor* getBattery(void);
};


#endif /* AC_FACADE_H_ */
