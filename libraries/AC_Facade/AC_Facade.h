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
    static bool land();
    static bool navigateTo(const Vector3f& destination);

    static float getAltitude();
    static bool isLanded();
    static bool areMotorsArmed();
    static bool destinationReached(const Vector3f& destination);
    static bool takeOffComplete(float altitude);
    static const Vector3f& getPosition();

private:
};


#endif /* AC_FACADE_H_ */
