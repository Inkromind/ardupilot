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
        return (AP_BattMonitor*) mock().actualCall("getBattery").onObject(this).returnPointerValue();
    }

    bool isLanded(void) {
        return (bool) mock().actualCall("isLanded").onObject(this).returnIntValue();
    }

    bool areMotorsArmed(void) {
        return (bool) mock().actualCall("areMotorsArmed").onObject(this).returnIntValue();
    }

    bool land(void) {
        return (bool) mock().actualCall("land").onObject(this).returnIntValue();
    }

    bool disarmMotors(void) {
        return (bool) mock().actualCall("disarmMotors").onObject(this).returnIntValue();
    }

    static void setFacade(ReactiveFacadeMock* facade) {
        AC_ReactiveFacade::reactiveFacade = facade;
    }
    static void deleteFacade() {
        AC_ReactiveFacade::reactiveFacade = 0;
    }

    Vector3f getRelativePosition(void) {
        return *((Vector3f*) mock().actualCall("getRelativePosition").onObject(this).returnPointerValue());
    }

    Vector3f getRealPosition(void) {
        return *((Vector3f*) mock().actualCall("getRealPosition").onObject(this).returnPointerValue());
    }
};

#endif /* MOCKS_REACTIVEFACADEMOCK_H_ */
