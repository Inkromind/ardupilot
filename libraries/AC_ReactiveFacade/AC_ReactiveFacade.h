/*
 * AC_ReactiveFacade.h
 *
 *  Created on: 31-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_REACTIVEFACADE_H_
#define AC_REACTIVEFACADE_H_

#include <AC_Facade.h>

class AC_ReactiveFacade : public AC_Facade {

public:
    virtual bool takeOff(float altitude);
    virtual bool land(void);
    virtual bool navigateTo(const Vector3f& destination);
    virtual bool navigateToAltitude(float altitude);
    virtual bool armMotors(void);
    virtual bool disarmMotors(void);
    virtual bool loiter(void);

    virtual AP_BattMonitor* getBattery(void);

    static AC_ReactiveFacade* getReactiveFacade(void) {
        if (!reactiveFacade)
            reactiveFacade = new AC_ReactiveFacade();
        return reactiveFacade;
    }

private:
    static AC_ReactiveFacade* reactiveFacade;
protected:
    AC_ReactiveFacade() {}
    virtual ~AC_ReactiveFacade() {}
};



#endif /* AC_REACTIVEFACADE_H_ */
