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
    virtual bool takeOff(float altitude);
    virtual bool land(void);
    virtual bool navigateTo(const Vector3f& destination);
    virtual bool navigateToAltitude(float altitude);
    virtual bool armMotors(void);
    virtual bool disarmMotors(void);
    virtual bool loiter(void);
    virtual bool initFlightMode(void);

    virtual void sendDebug(const prog_char_t *str);

    virtual float getAltitude(void);
    virtual bool isLanded(void);
    virtual bool areMotorsArmed(void);
    virtual bool destinationReached(const Vector3f& destination);
    virtual bool altitudeReached(float altitude);
    virtual Vector3f getPosition(void);

    virtual uint32_t getCH8Position(void);
    virtual uint32_t getCH7Position(void);

    static AC_Facade* getFacade(void) {
        if (!facade)
            facade = new AC_Facade();
        return facade;
    }

private:
    static AC_Facade* facade;
protected:
    AC_Facade() {}
    virtual ~AC_Facade() {}
};


#endif /* AC_FACADE_H_ */
