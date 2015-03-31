/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>
#include <AC_ReactiveFacade.h>
#include <AC_SafetyControl.h>
#include "control_mad.h"

AC_Facade* AC_Facade::facade = 0;
AC_ReactiveFacade* AC_ReactiveFacade::reactiveFacade = 0;

#define NEAR_DESTINATION_RADIUS     100.0f

bool MAD_navInitialized = false;
Vector3f MAD_origin;

void MAD_updateOrigin(void);
bool MAD_inControl(void);
bool MAD_relativeDestinationReached(const Vector3f& destination);

bool MAD_inControl() {
    if (control_mode != MAD)
        return false;
    return !(AC_SafetyControl::getInstance()->isActive());
}

bool AC_Facade::initFlightMode() {
    if (control_mode == MAD)
        return true;
    return set_mode(MAD);
}

bool AC_ReactiveFacade::takeOff(float altitude) {
    if (!initFlightMode())
        return false;
    return mad_takeoff_start(altitude);
}
bool AC_Facade::takeOff(float altitude) {
    if (!MAD_inControl()) {
        return false;
    }
    return mad_takeoff_start(altitude);
}

bool AC_ReactiveFacade::land() {
    if (!initFlightMode())
        return false;
    return mad_land_start();
}

bool AC_Facade::land() {
    if (!MAD_inControl())
        return false;
    return mad_land_start();
}

bool AC_ReactiveFacade::navigateTo(const Vector3f& destination) {
    if (!initFlightMode())
        return false;
    return mad_nav_start(destination + MAD_origin);
}
bool AC_Facade::navigateTo(const Vector3f& destination) {
    if (!MAD_inControl())
        return false;
    return mad_nav_start(destination + MAD_origin);
}

bool AC_ReactiveFacade::navigateToAltitude(float altitude) {
    if (!initFlightMode())
        return false;
    return mad_nav_altitude_start(altitude);
}
bool AC_Facade::navigateToAltitude(float altitude) {
    if (!MAD_inControl())
        return false;
    return mad_nav_altitude_start(altitude);
}

bool AC_ReactiveFacade::armMotors() {
    if (!initFlightMode())
        return false;
    return mad_arm_motors();
}
bool AC_Facade::armMotors() {
    if (!MAD_inControl())
        return false;
    return mad_arm_motors();
}

bool AC_ReactiveFacade::disarmMotors() {
    if (!initFlightMode())
        return false;
    return mad_disarm_motors();
}
bool AC_Facade::disarmMotors() {
    if (!MAD_inControl())
        return false;
    return mad_disarm_motors();
}

bool AC_ReactiveFacade::loiter() {
    if (!initFlightMode())
        return false;
    return mad_loiter_start(false);
}
bool AC_Facade::loiter() {
    if (!MAD_inControl())
        return false;
    return mad_loiter_start(false);
}

float AC_Facade::getAltitude() {
    return inertial_nav.get_altitude();
}

bool AC_Facade::altitudeReached(float altitude) {
    Vector3f dest = getPosition();
    dest.z = altitude;

    return MAD_relativeDestinationReached(dest);
}

bool AC_Facade::isLanded() {
    return ap.land_complete;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(const Vector3f& destination) {
    if (control_mode != MAD)
        return false;

    Vector3f relativeDestination = destination + MAD_origin;

    return MAD_relativeDestinationReached(relativeDestination);
}

bool MAD_relativeDestinationReached(const Vector3f& destination) {
    if (control_mode != MAD)
        return false;

    Vector3f currentDistToDest = inertial_nav.get_position() - destination;
    if (currentDistToDest.length() > NEAR_DESTINATION_RADIUS) {
        return false;
    }

    if (mad_mode == Mad_Land)
        return false;

    Vector3f wpDistToDest = wp_nav.get_wp_destination() - destination;

    return (wpDistToDest.length() <= NEAR_DESTINATION_RADIUS);
}

Vector3f AC_Facade::getPosition() {
    return inertial_nav.get_position();
}

void AC_Facade::sendDebug(const prog_char_t *str) {
    gcs_send_text_P(SEVERITY_LOW, str);
}

uint32_t AC_Facade::getCH8Position(void) {
    return ap.CH8_flag;
}

uint32_t AC_Facade::getCH7Position(void) {
    return ap.CH7_flag;
}

AP_BattMonitor* AC_ReactiveFacade::getBattery(void) {
    return &battery;
}

void MAD_updateOrigin(void) {
    if (MAD_navInitialized)
        MAD_origin = MAD_origin - inertial_nav.get_position();
    else
        MAD_navInitialized = true;
}

