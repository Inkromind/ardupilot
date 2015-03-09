/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>
#include "control_mad.h"

static bool inControl() {
    return (control_mode == MAD);
}

bool AC_Facade::takeOff(float altitude) {
    if (!inControl())
        return false;
    return mad_takeoff_start(altitude);
}

bool AC_Facade::land() {
    if (!inControl())
        return false;
    return mad_land_start();
}

bool AC_Facade::navigateTo(const Vector3f& destination) {
    if (!inControl())
        return false;
    return mad_nav_start(destination);
}

float AC_Facade::getAltitude() {
    return inertial_nav.get_altitude();
}

bool AC_Facade::takeOffComplete(float altitude) {
    if (!wp_nav.reached_wp_destination())
        return false;
    Vector3f wp = wp_nav.get_wp_destination();
    return (wp.z == altitude);
}

bool AC_Facade::isLanded() {
    return ap.land_complete;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(const Vector3f& destination) {
    if (!wp_nav.reached_wp_destination())
        return true;
    Vector3f wp = wp_nav.get_wp_destination();
    return (destination == wp);
}

const Vector3f& AC_Facade::getPosition() {
    return inertial_nav.get_position();
}
