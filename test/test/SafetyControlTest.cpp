/*
 * SafetyControlTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AC_SafetyControl/AC_SafetyControl.h"

//Mocks
#include  "../mocks/BehaviourMock.h"


class SafetyControlStub : public AC_SafetyControl {
public:
    SafetyControlStub() : AC_SafetyControl() {}

    AMW_List<AC_Behaviour*>* getBehaviours() {
        return &behaviours;
    }
};

SafetyControlStub* module = 0;

TEST_GROUP(SafetyControl)
{
    void setup()
    {
        module = new SafetyControlStub();
        AC_Behaviour* behaviour = module->getBehaviours()->front();
        module->getBehaviours()->pop_front();
        delete behaviour;
        behaviour = 0;
    }
    void teardown()
    {
        delete module;
        module = 0;
    }
};

TEST(SafetyControl, InitTest) {
    SafetyControlStub stub;
    CHECK_EQUAL(1, stub.getBehaviours()->size());
    CHECK_FALSE(stub.isActive());
}

TEST(SafetyControl, RunNoBehaviours) {
    module->run();
    CHECK_FALSE(module->isActive());
}

TEST(SafetyControl, RunFirstBehaviour) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();

    module->getBehaviours()->push_back(behaviour1);
    module->getBehaviours()->push_back(behaviour2);
    module->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour1).andReturnValue(true);
    module->run();
    CHECK_TRUE(module->isActive());
}

TEST(SafetyControl, RunLastBehaviour) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();

    module->getBehaviours()->push_back(behaviour1);
    module->getBehaviours()->push_back(behaviour2);
    module->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour2).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour2).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour3).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour3).andReturnValue(true);
    module->run();
    CHECK_TRUE(module->isActive());
}

TEST(SafetyControl, RunNoBehaviourActive) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();

    module->getBehaviours()->push_back(behaviour1);
    module->getBehaviours()->push_back(behaviour2);
    module->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour2).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour2).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour3).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour3).andReturnValue(false);
    module->run();
    CHECK_FALSE(module->isActive());
}
