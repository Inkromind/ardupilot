/*
 * MAD_Facade.h
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_FACADE_H_
#define MAD_FACADE_H_

class MAD_Facade {

public:
    static bool inControl();

    static bool startTakeOff(uint8_t altitude);
    static bool startLand();
    static bool armMotors();
    static bool disarmMotors();
    static bool startNav(Vector3f* destination);

    static uint8_t getAltitude();
    static bool isLanded();
    static bool areMotorsArmed();
    static bool destinationReached(Vector3f* destination);


private:
    MAD_Facade();
};

#endif /* MAD_FACADE_H_ */
