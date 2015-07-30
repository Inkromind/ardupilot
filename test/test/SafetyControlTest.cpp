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

SafetyControlStub* SCmodule = 0;

TEST_GROUP(SafetyControl)
{
    void setup()
    {
        SCmodule = new SafetyControlStub();
        AC_Behaviour* behaviour = SCmodule->getBehaviours()->front();
        SCmodule->getBehaviours()->pop_front();
        delete behaviour;
        behaviour = 0;
    }
    void teardown()
    {
        delete SCmodule;
        SCmodule = 0;
    }
};

TEST(SafetyControl, ConstructorTest) {
    SafetyControlStub stub;

    CHECK_EQUAL(1, stub.getBehaviours()->size());
    CHECK_FALSE(stub.isActive());
}

TEST(SafetyControl, RunNoBehaviours) {
    SCmodule->run();

    CHECK_FALSE(SCmodule->isActive());
}

TEST(SafetyControl, RunFirstBehaviour) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();
    SCmodule->getBehaviours()->push_back(behaviour1);
    SCmodule->getBehaviours()->push_back(behaviour2);
    SCmodule->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour1).andReturnValue(true);

    SCmodule->run();

    CHECK_TRUE(SCmodule->isActive());
}

TEST(SafetyControl, RunLastBehaviour) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();
    SCmodule->getBehaviours()->push_back(behaviour1);
    SCmodule->getBehaviours()->push_back(behaviour2);
    SCmodule->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour2).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour2).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour3).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour3).andReturnValue(true);

    SCmodule->run();

    CHECK_TRUE(SCmodule->isActive());
}

TEST(SafetyControl, RunNoBehaviourActive) {
    BehaviourMock* behaviour1 = new BehaviourMock();
    BehaviourMock* behaviour2 = new BehaviourMock();
    BehaviourMock* behaviour3 = new BehaviourMock();
    SCmodule->getBehaviours()->push_back(behaviour1);
    SCmodule->getBehaviours()->push_back(behaviour2);
    SCmodule->getBehaviours()->push_back(behaviour3);

    mock().expectOneCall("BisActive").onObject(behaviour1).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour2).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour2).andReturnValue(false);
    mock().expectOneCall("BisActive").onObject(behaviour3).andReturnValue(true);
    mock().expectOneCall("Bperform").onObject(behaviour3).andReturnValue(false);

    SCmodule->run();

    CHECK_FALSE(SCmodule->isActive());
}
