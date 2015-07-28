/*
 * AC_ReactiveFacade.h
 *
 *  Created on: 31-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_REACTIVEFACADE_H_
#define AC_REACTIVEFACADE_H_

#include <AC_Facade.h>
#include <AP_BattMonitor.h>

/**
 * A facade allowing access to ArduCopter.
 * Commands executed through this facade have a high priority.
 */
class AC_ReactiveFacade : public AC_Facade {

public:
    /**
     * Arm motors and takeoff to the given altitude.
     * Flight mode will be initialized if necessary.
     *
     * @param float altitude - Target altitude (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool takeOff(float altitude);

    /**
     * Land
     * Flight mode will be initialized if necessary.
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool land(void);

    /**
     * Navigate to the given coordinates
     * Flight mode will be initialized if necessary.
     *
     * @param destination - 3D Destination (relative to origin) (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool navigateTo(const Vector3f& destination);

    /**
     * Navigate to the given altitude
     * Flight mode will be initialized if necessary.
     *
     * @param altitude - Target altitude (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool navigateToAltitude(float altitude);

    /**
     * Arm motors
     * Flight mode will be initialized if necessary.
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool armMotors(void);

    /**
     * Disarm motors
     * Flight mode will be initialized if necessary.
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool disarmMotors(void);

    /**
     * Loiter at the given location
     * Flight mode will be initialized if necessary.
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool loiter(void);

    /**
     * Return the instance of the reactive facade
     */
    static AC_ReactiveFacade* getReactiveFacade(void) {
        if (!reactiveFacade)
            reactiveFacade = new AC_ReactiveFacade();
        return reactiveFacade;
    }

private:
protected:
    static AC_ReactiveFacade* reactiveFacade;
    AC_ReactiveFacade() {}
    virtual ~AC_ReactiveFacade() {}
};



#endif /* AC_REACTIVEFACADE_H_ */
