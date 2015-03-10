/*
 * AC_Facade.h
 *
 *  Created on: 27-feb.-2015
 *      Author: Arne
 */

#ifndef AC_FACADE_H_
#define AC_FACADE_H_


class AC_Facade {

public:
    static bool takeOff(float altitude) = 0;
    static bool land(void) = 0;
    static bool navigateTo(const Vector3f& destination) = 0;
    static bool armMotors(void) = 0;
    static bool disarmMotors(void) = 0;

    static float getAltitude(void) = 0;
    static bool isLanded(void) = 0;
    static bool areMotorsArmed(void) = 0;
    static bool destinationReached(const Vector3f& destination) = 0;
    static bool takeOffComplete(float altitude) = 0;
    static const Vector3f& getPosition(void) = 0;

};


#endif /* AC_FACADE_H_ */
