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
        return *((Vector3f*) mock().actualCall("FgetRealPosition").onObject(this).returnPointerValue());
    }

    uint32_t getTimeMillis(void) {
        return (uint32_t) mock().actualCall("FgetTimeMillis").onObject(this).returnIntValue();
    }
};

#endif /* MOCKS_FACADEMOCK_H_ */
