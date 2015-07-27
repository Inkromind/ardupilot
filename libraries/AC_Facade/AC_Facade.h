/*
 * AC_Facade.h
 *
 *  Created on: 27-feb.-2015
 *      Author: Arne
 */

#ifndef AC_FACADE_H_
#define AC_FACADE_H_

#include "../AP_Math/vector3.h"
#ifndef TESTENV
    #include <AP_Progmem.h>
#endif
//#include <AP_HAL.h>
#include <AP_BattMonitor.h>

#define NEAR_DESTINATION_RADIUS     20.0f

class AC_Facade {

public:
    virtual bool takeOff(float altitude);
    virtual bool land(void);
    virtual bool navigateTo(const Vector3f& destination);
    virtual bool navigateToAltitude(float altitude);
    virtual bool armMotors(void);
    virtual bool disarmMotors(void);
    virtual bool loiter(void);
    virtual bool initFlightMode(void);
    virtual float getAltitude(void);
    virtual bool isLanded(void);
    virtual bool areMotorsArmed(void);
    virtual bool destinationReached(const Vector3f& destination, float radius = NEAR_DESTINATION_RADIUS);
    virtual bool altitudeReached(float altitude);
    virtual Vector3f getRelativePosition(void);
    virtual Vector3f getRealPosition(void);

    virtual uint32_t getCH8Position(void);
    virtual uint32_t getCH7Position(void);

    static AC_Facade* getFacade(void) {
        if (!facade)
            facade = new AC_Facade();
        return facade;
    }

    virtual AP_BattMonitor* getBattery(void);

private:
    static AC_Facade* facade;
protected:
    AC_Facade() {}
    virtual ~AC_Facade() {}
};


#endif /* AC_FACADE_H_ */
