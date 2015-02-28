/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>

bool AC_Facade::inControl() {
    return false;
}

bool AC_Facade::startTakeOff(uint8_t altitude) {
    return false;
}

bool AC_Facade::startLand() {
    return false;
}

bool AC_Facade::armMotors() {
    return false;
}

bool AC_Facade::disarmMotors() {
    return false;
}

bool AC_Facade::startNav(Vector3f* destination) {
    return false;
}

uint8_t AC_Facade::getAltitude() {
    return 0;
}

bool AC_Facade::isLanded() {
    return false;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(Vector3f* destination) {
    return false;
}
