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
    static bool takeOff(float altitude);
    static bool land(void);
    static bool navigateTo(const Vector3f& destination);
    static bool armMotors(void);
    static bool disarmMotors(void);

    static float getAltitude(void);
    static bool isLanded(void);
    static bool areMotorsArmed(void);
    static bool destinationReached(const Vector3f& destination);
    static bool takeOffComplete(float altitude);
    static const Vector3f& getPosition(void);

};


#endif /* AC_FACADE_H_ */
