/*
 * CommandMock.h
 *
 *  Created on: 30-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_COMMANDMOCK_H_
#define MOCKS_COMMANDMOCK_H_

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "../../libraries/AMW_Planner/AMW_Command.h"

class CommandMock : public AMW_Command {
    void run(bool attempt) {
        mock("Command").actualCall("run").onObject(this).withParameter("attempt", attempt);
    }

    bool isComplete(void) const {
        return mock("Command").actualCall("isComplete").onObject(const_cast<CommandMock*>(this)).returnIntValueOrDefault(1);
    }

    bool hasStarted(void) const {
        return mock("Command").actualCall("hasStarted").onObject(const_cast<CommandMock*>(this)).returnIntValueOrDefault(1);
    }

    void pause(void) {
        mock("Command").actualCall("pause").onObject(this);
    };

    void resume(void) {
        mock("Command").actualCall("resume").onObject(this);
    };

    bool hasFailed(void) const {
        return mock("Command").actualCall("hasFailed").onObject(const_cast<CommandMock*>(this)).returnIntValueOrDefault(0);
    }
};



#endif /* MOCKS_COMMANDMOCK_H_ */
