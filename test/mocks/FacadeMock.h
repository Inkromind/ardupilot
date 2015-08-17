/*
 * FacadeMock.h
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_FACADEMOCK_H_
#define MOCKS_FACADEMOCK_H_

#include <AC_Facade.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class FacadeMock : public AC_Facade {
public:
    static void setFacade(FacadeMock* facade) {
        AC_Facade::facade = facade;
    }
    static void deleteFacade() {
        AC_Facade::facade = 0;
    }

    Vector3f getRealPosition(void) {
        return *((Vector3f*) mock("Facade").actualCall("getRealPosition").onObject(this).returnPointerValue());
    }

    float getAltitude(void) {
        return (float) mock("Facade").actualCall("getAltitude").onObject(this).returnDoubleValueOrDefault(0.0);
    }

    uint32_t getTimeMillis(void) {
        return (uint32_t) mock("Facade").actualCall("getTimeMillis").onObject(this).returnIntValueOrDefault(0);
    }

    AP_BattMonitor* getBattery(void) {
        return (AP_BattMonitor*) mock("Facade").actualCall("getBattery").onObject(this).returnPointerValue();
    }

    bool destinationReached(const Vector3f& destination, float radius) {
        return mock("Facade").actualCall("destinationReached").onObject(this).withDoubleParameter("destinationX", destination.x).
                withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", destination.z).returnIntValueOrDefault(1);
    }

    bool isLanded(void) {
        return (bool) mock("Facade").actualCall("isLanded").onObject(this).returnIntValueOrDefault(1);
    }

    bool areMotorsArmed(void) {
        return (bool) mock("Facade").actualCall("areMotorsArmed").onObject(this).returnIntValueOrDefault(0);
    }

    bool land(void) {
        return (bool) mock("Facade").actualCall("land").onObject(this).returnIntValueOrDefault(1);
    }

    bool disarmMotors(void) {
        return (bool) mock("Facade").actualCall("disarmMotors").onObject(this).returnIntValueOrDefault(1);
    }

    bool takeOff(float altitude) {
        return (bool) mock("Facade").actualCall("takeoff").onObject(this).withDoubleParameter("altitude", altitude).returnIntValueOrDefault(1);
    }

    bool navigateTo(const Vector3f& destination) {
        return (bool) mock("Facade").actualCall("navigateTo").onObject(this).withDoubleParameter("destinationX", destination.x).
                withDoubleParameter("destinationY", destination.y).withDoubleParameter("destinationZ", destination.z).returnIntValueOrDefault(1);
    }

    bool navigateToAltitude(float altitude) {
        return (bool) mock("Facade").actualCall("navigateToAltitude").onObject(this).withDoubleParameter("altitude", altitude).returnIntValueOrDefault(1);
    }

    bool armMotors() {
        return (bool) mock("Facade").actualCall("armMotors").onObject(this).returnIntValueOrDefault(1);
    }

    bool loiter() {
        return (bool) mock("Facade").actualCall("loiter").onObject(this).returnIntValueOrDefault(1);
    }

    bool altitudeReached(float altitude) {
        return (bool) mock("Facade").actualCall("altitudeReached").onObject(this).withDoubleParameter("altitude", altitude).returnIntValueOrDefault(1);
    }
};

#endif /* MOCKS_FACADEMOCK_H_ */
