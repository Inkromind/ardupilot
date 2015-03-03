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
    static bool inControl();

    static bool startTakeOff(float altitude);
    static bool startLand();
    static bool armMotors();
    static bool disarmMotors();
    static bool startNav(const Vector3f& destination);

    static uint8_t getAltitude();
    static bool isLanded();
    static bool areMotorsArmed();
    static bool destinationReached(const Vector3f& destination);


private:
};


#endif /* AC_FACADE_H_ */
