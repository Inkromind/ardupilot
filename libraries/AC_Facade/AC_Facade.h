/*
 * AC_Facade.h
 *
 *  Created on: 27-feb.-2015
 *      Author: Arne
 */

#ifndef AC_FACADE_H_
#define AC_FACADE_H_

#include "../AP_Math/vector3.h"
#include <AP_BattMonitor.h>
#include <stdint.h>

// Default acceptance radius for nav
#define NEAR_DESTINATION_RADIUS     20.0f

/**
 * A facade allowing access to ArduCopter.
 * Commands executed through this facade have a low priority.
 */
class AC_Facade {

public:
    /**
     * Arm motors and takeoff to the given altitude
     *
     * @param float altitude - Target altitude (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool takeOff(float altitude);

    /**
     * Land
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool land(void);

    /**
     * Navigate to the given coordinates
     *
     * @param destination - 3D Destination (relative to origin) (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool navigateTo(const Vector3f& destination);

    /**
     * Navigate to the given altitude
     *
     * @param altitude - Target altitude (in cm)
     * @return True if command is being performed. False otherwise
     */
    virtual bool navigateToAltitude(float altitude);

    /**
     * Arm motors
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool armMotors(void);

    /**
     * Disarm motors
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool disarmMotors(void);

    /**
     * Loiter at the given location
     *
     * @return True if command is being performed. False otherwise
     */
    virtual bool loiter(void);

    /**
     * Initialize the custom flight mode
     *
     * @return True if the flight mode has been initialized. False otherwise
     */
    virtual bool initFlightMode(void);

    /**
     * Get the current altitude
     *
     * @return Current altitude (in cm)
     */
    virtual float getAltitude(void);

    /**
     * Check if the drone is currently landed
     *
     * @return True if landed. False otherwise
     */
    virtual bool isLanded(void);

    /**
     * Check if the motors are armed
     *
     * @return True if the motors are armed. False otherwise
     */
    virtual bool areMotorsArmed(void);

    /**
     * Check if the given destination has been reached.
     *
     * @param destination - 3D destination (relative to origin) (in cm)
     * @param radius (optional) - Acceptance radius (in cm)
     * @return True if the destination has been reached. False otherwise
     */
    virtual bool destinationReached(const Vector3f& destination, float radius = NEAR_DESTINATION_RADIUS);


    /**
     * Check if the given altitude has been reached. The default acceptance radius is used.
     *
     * @param altitude - Target altitude (in cm)
     * @return True if the destination has been reached. False otherwise
     */
    virtual bool altitudeReached(float altitude);

    /**
     * Get the current position in relation to the last takeoff location.
     *
     * @return 3D Location (in cm) in relation to last takeoff location.
     */
    virtual Vector3f getRelativePosition(void);

    /**
     * Get the current position in relation to the origin.
     *
     * @return 3D Location (in cm) in relation to the origin.
     */
    virtual Vector3f getRealPosition(void);

    /**
     * Get the position of the lever on Channel 8
     */
    virtual uint32_t getCH8Position(void);

    /**
     * Get the position of the lever on Channel 7
     */
    virtual uint32_t getCH7Position(void);

    /**
     * Return the instance of the facade
     */
    static AC_Facade* getFacade(void) {
        if (!facade)
            facade = new AC_Facade();
        return facade;
    }

    /**
     * Get the battery monitor
     */
    virtual AP_BattMonitor* getBattery(void);

    /**
     * Get the current scheduler time in milliseconds
     */
    virtual uint32_t getTimeMillis(void);

private:
protected:
    static AC_Facade* facade;
    AC_Facade() {}
    virtual ~AC_Facade() {}
};


#endif /* AC_FACADE_H_ */
