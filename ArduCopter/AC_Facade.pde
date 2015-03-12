/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>
#include "control_mad.h"

#define NEAR_DESTINATION_RADIUS     10.0f

bool MAD_inControl(void);

static bool MAD_inControl() {
    return (control_mode == MAD);
}

bool AC_Facade::takeOff(float altitude) {
    if (!MAD_inControl())
        return false;
    return mad_takeoff_start(altitude);
}

bool AC_Facade::land() {
    if (!MAD_inControl())
        return false;
    return mad_land_start();
}

bool AC_Facade::navigateTo(const Vector3f& destination) {
    if (!MAD_inControl())
        return false;
    return mad_nav_start(destination);
}

bool AC_Facade::armMotors() {
    if (!MAD_inControl())
        return false;
    return mad_arm_motors();
}

bool AC_Facade::disarmMotors() {
    if (!MAD_inControl())
        return false;
    return mad_disarm_motors();
}

float AC_Facade::getAltitude() {
    return inertial_nav.get_altitude();
}

bool AC_Facade::takeOffComplete(float altitude) {
    Vector3f dest = AC_Facade::getPosition();
    dest.y = altitude;

    return AC_Facade::destinationReached(dest);
}

bool AC_Facade::isLanded() {
    return ap.land_complete;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(const Vector3f& destination) {
    Vector3f currentDistToDest = inertial_nav.get_position() - destination;
    if (currentDistToDest.length() > NEAR_DESTINATION_RADIUS) {
        return false;
    }
    Vector3f wpDistToDest = wp_nav.get_wp_destination() - destination;
    return (wpDistToDest.length() <= NEAR_DESTINATION_RADIUS);
}

Vector3f AC_Facade::getPosition() {
    return Vector3f() + inertial_nav.get_position();
}
