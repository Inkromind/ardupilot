/*
 * AC_Facade.cpp
 *
 *  Created on: 8-jul.-2015
 *      Author: Arne
 */

#include <AC_Facade.h>
#include <AC_ReactiveFacade.h>

AC_Facade* AC_Facade::facade = 0;
AC_ReactiveFacade* AC_ReactiveFacade::reactiveFacade = 0;

bool AC_Facade::initFlightMode() {
    return true;
}

bool AC_ReactiveFacade::takeOff(float altitude) {
    return true;
}
bool AC_Facade::takeOff(float altitude) {
    return true;
}

bool AC_ReactiveFacade::land() {
    return true;
}

bool AC_Facade::land() {
    return true;
}

bool AC_ReactiveFacade::navigateTo(const Vector3f& destination) {
    return true;
}
bool AC_Facade::navigateTo(const Vector3f& destination) {
    return true;
}

bool AC_ReactiveFacade::navigateToAltitude(float altitude) {
    return true;
}
bool AC_Facade::navigateToAltitude(float altitude) {
    return true;
}

bool AC_ReactiveFacade::armMotors() {
    return true;
}
bool AC_Facade::armMotors() {
    return true;
}

bool AC_ReactiveFacade::disarmMotors() {
    return true;
}
bool AC_Facade::disarmMotors() {
    return true;
}

bool AC_ReactiveFacade::loiter() {
    return true;
}
bool AC_Facade::loiter() {
    return true;
}

float AC_Facade::getAltitude() {
    return 0.0;
}

bool AC_Facade::altitudeReached(float altitude, float radius) {
    return true;
}

bool AC_Facade::isLanded() {
    return true;
}

bool AC_Facade::areMotorsArmed() {
    return true;
}

bool AC_Facade::destinationReached(const Vector3f& destination, float radius) {
    return true;
}

bool MAD_relativeDestinationReached(const Vector3f& destination, float radius) {
    return true;
}

Vector3f AC_Facade::getRelativePosition() {
    return Vector3f();
}

Vector3f AC_Facade::getRealPosition() {
    return Vector3f();
}

uint32_t AC_Facade::getCH8Position(void) {
    return 0;
}

uint32_t AC_Facade::getCH7Position(void) {
    return 0;
}

AP_BattMonitor* AC_Facade::getBattery(void) {
    return 0;
}

uint32_t AC_Facade::getTimeMillis(void) {
    return 0;
}

DataFlash_Class* AC_Facade::getDataFlash(void) {
    return 0;
}
