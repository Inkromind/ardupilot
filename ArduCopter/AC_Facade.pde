/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_Facade.h>
#include <AC_ReactiveFacade.h>
#include <AC_SafetyControl.h>
#include "control_mad.h"
#include <AC_CommunicationFacade.h>
#include <AMW_Facade.h>

AC_Facade* AC_Facade::facade = 0;
AC_ReactiveFacade* AC_ReactiveFacade::reactiveFacade = 0;

bool MAD_navInitialized = false;

// Takeoff location relative to origin
Vector2f MAD_takeoffLocation;
// Homebase relative to origin
Vector2f MAD_homebase;

void MAD_updateTakeoffLocation(void);
void MAD_resetHomebase(void);
void MAD_setHomebase(float x, float y);
bool MAD_inControl(void);
bool MAD_relativeDestinationReached(const Vector3f& destination, float radius = NEAR_DESTINATION_RADIUS);
void MAD_runCorridorBroadcast(void);
Vector3f MAD_convertExternToIntern(Vector3f position) {
    Vector2f pos2d = Vector2f(position.x, position.y);
    Vector2f internPos = Vector2f(position.x, position.y) - MAD_takeoffLocation;
    return Vector3f(internPos.x, internPos.y, position.z);
}
Vector3f MAD_convertInternToExtern(Vector3f position) {
    Vector2f externPos = Vector2f(position.x, position.y) + MAD_takeoffLocation;
    return Vector3f(externPos.x, externPos.y, position.z);
}

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
    return mad_nav_start(MAD_convertExternToIntern(destination));
}
bool AC_Facade::navigateTo(const Vector3f& destination) {
    if (!MAD_inControl())
        return false;
    return mad_nav_start(MAD_convertExternToIntern(destination));
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

bool AC_Facade::altitudeReached(float altitude, float radius) {
    Vector3f dest = getRelativePosition();

    if (abs(dest.z - altitude) > radius)
        return false;

    return MAD_relativeDestinationReached(dest, radius);
}

bool AC_Facade::isLanded() {
    return ap.land_complete;
}

bool AC_Facade::areMotorsArmed() {
    return motors.armed();
}

bool AC_Facade::destinationReached(const Vector3f& destination, float radius) {
    if (control_mode != MAD)
        return false;

    Vector3f relativeDestination = MAD_convertExternToIntern(destination);

    return MAD_relativeDestinationReached(relativeDestination, radius);
}

bool MAD_relativeDestinationReached(const Vector3f& destination, float radius) {
    if (control_mode != MAD)
        return false;

    Vector3f currentDistToDest = inertial_nav.get_position() - destination;
    if (currentDistToDest.length() > radius) {
        return false;
    }

    if (mad_mode == Mad_Land)
        return (ap.land_complete);

    Vector3f wpDistToDest = wp_nav.get_wp_destination() - destination;
    if (wpDistToDest.length() > radius)
        return false;

    Vector3f wpDistToCurrent = wp_nav.get_wp_destination() - inertial_nav.get_position();

    return (wpDistToCurrent.length() <= radius);
}

Vector3f AC_Facade::getRelativePosition() {
    return inertial_nav.get_position();
}

Vector3f AC_Facade::getRealPosition() {
    return MAD_convertInternToExtern(inertial_nav.get_position());
}

uint32_t AC_Facade::getCH8Position(void) {
    return ap.CH8_flag;
}

uint32_t AC_Facade::getCH7Position(void) {
    return ap.CH7_flag;
}

AP_BattMonitor* AC_Facade::getBattery(void) {
    return &battery;
}

uint32_t AC_Facade::getTimeMillis(void) {
    return hal.scheduler->millis();
}

DataFlash_Class* AC_Facade::getDataFlash(void) {
    return &DataFlash;
}

void MAD_updateTakeoffLocation(void) {
    if (MAD_navInitialized)
        MAD_takeoffLocation = MAD_takeoffLocation + Vector2f(inertial_nav.get_position().x, inertial_nav.get_position().y);
    else {
        MAD_takeoffLocation = Vector2f(inertial_nav.get_position().x, inertial_nav.get_position().y);
        MAD_navInitialized = true;
    }
}
void MAD_setHomebase(float x, float y) {
    MAD_homebase = Vector2f(x, y);
    AMW_Facade::setHomebase(Vector2f(x, y));
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Setting homebase to <%.2f, %.2f>"), x, y);
}
void MAD_resetHomebase(void) {
    MAD_setHomebase(AC_Facade::getFacade()->getRealPosition().x, AC_Facade::getFacade()->getRealPosition().y);
}
void MAD_runCorridorBroadcast(void) {
    AMW_Facade::runCorridorBroadcast();
}

