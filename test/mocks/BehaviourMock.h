/*
 * BehaviourMock.h
 *
 *  Created on: 29-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_BEHAVIOURMOCK_H_
#define MOCKS_BEHAVIOURMOCK_H_

#include "../../libraries/AC_SafetyControl/AC_Behaviour.h"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class BehaviourMock : public AC_Behaviour {
    bool isActive(void) {
        return mock("Behaviour").actualCall("isActive").onObject(this).returnIntValueOrDefault(1);
    }

    bool perform(void) {
        return mock("Behaviour").actualCall("perform").onObject(this).returnIntValueOrDefault(1);
    }
};



#endif /* MOCKS_BEHAVIOURMOCK_H_ */
