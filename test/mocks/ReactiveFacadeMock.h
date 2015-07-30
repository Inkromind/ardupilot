/*
 * ReactiveFacadeMock.h
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_REACTIVEFACADEMOCK_H_
#define MOCKS_REACTIVEFACADEMOCK_H_

#include <AC_ReactiveFacade.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class ReactiveFacadeMock : public AC_ReactiveFacade {
public:
    AP_BattMonitor* getBattery(void) {
        return (AP_BattMonitor*) mock("ReactiveFacade").actualCall("getBattery").onObject(this).returnPointerValue();
    }

    bool isLanded(void) {
        return (bool) mock("ReactiveFacade").actualCall("isLanded").onObject(this).returnIntValueOrDefault(1);
    }

    bool areMotorsArmed(void) {
        return (bool) mock("ReactiveFacade").actualCall("areMotorsArmed").onObject(this).returnIntValueOrDefault(0);
    }

    bool land(void) {
        return (bool) mock("ReactiveFacade").actualCall("land").onObject(this).returnIntValueOrDefault(1);
    }

    bool disarmMotors(void) {
        return (bool) mock("ReactiveFacade").actualCall("disarmMotors").onObject(this).returnIntValueOrDefault(1);
    }

    static void setFacade(ReactiveFacadeMock* facade) {
        AC_ReactiveFacade::reactiveFacade = facade;
    }
    static void deleteFacade() {
        AC_ReactiveFacade::reactiveFacade = 0;
    }

    Vector3f getRelativePosition(void) {
        return *((Vector3f*) mock("ReactiveFacade").actualCall("getRelativePosition").onObject(this).returnPointerValue());
    }

    Vector3f getRealPosition(void) {
        return *((Vector3f*) mock("ReactiveFacade").actualCall("getRealPosition").onObject(this).returnPointerValue());
    }
};

#endif /* MOCKS_REACTIVEFACADEMOCK_H_ */
