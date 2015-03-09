/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>

bool AC_Facade::inControl() {
    return (control_mode == MAD);
}

bool AC_Facade::takeOff(float altitude) {
    if (!inControl())
        return false;
    mad_takeoff_start(altitude);
    return true;
}

bool AC_Facade::land() {
    if (!inControl())
        return false;
    mad_land_start();
    return true;
}

bool AC_Facade::armMotors() {
    pre_arm_checks(true);
    if(ap.pre_arm_check && arm_checks(true)) {
        if (init_arm_motors()) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}

bool AC_Facade::disarmMotors() {
    init_disarm_motors();
    return true;
}

bool AC_Facade::navigateTo(const Vector3f& destination) {
    if (!inControl())
        return false;
    mad_nav_start(destination);
    return true;
}

uint8_t AC_Facade::getAltitude() {
    return 0;
}

bool AC_Facade::isLanded() {
    return ap.land_complete;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(const Vector3f& destination) {
    return false;
}
