/*
 * CommandCompositeTest.cpp
 *
 *  Created on: 28-jul.-2015
 *      Author: Arne
 */

//CppUTest
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

//Class being tested
#include "../../libraries/AMW_Planner/AMW_Command_Composite.h"

//Mocks
#include "../mocks/CommandMock.h"

class DummyCompositeCommand : public AMW_Command_Composite {
public:
    AMW_Queue<AMW_Command*>* getSubCommands() {
        return &subCommands;
    }
    void completedSubCommand() {
        mock("CommandComposite").actualCall("completedSubCommand").onObject(this);
    }

    void updateStatus(void) {
        mock("CommandComposite").actualCall("updateStatus").onObject(this);
    }

    void startCommand(bool attempt) {
        if (mock("CommandComposite").getData("setStartedCommand").getIntValue())
            commandStarted = true;
        mock("CommandComposite").actualCall("startCommand").onObject(this).withParameter("attempt", attempt);
    }

    void setCommandStarted(bool started) {
        commandStarted = started;
    }
    bool getCommandStarted() {
        return commandStarted;
    }
    void setCompleted(bool newCompleted) {
        completed = newCompleted;
    }
    void setFailed(bool newFailed) {
        failed = newFailed;
    }
    bool getPaused() {
        return paused;
    }
    void setPaused(bool newPaused) {
        paused = newPaused;
    }

};

DummyCompositeCommand* CCcommand = 0;

TEST_GROUP(CommandComposite)
{
    void setup()
    {
        CCcommand = new DummyCompositeCommand();
    }
    void teardown()
    {
        delete CCcommand;
    }
};

TEST(CommandComposite, runCompleted)
{
    CommandMock* subCommand = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->setCompleted(true);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);

    CCcommand->run(true);

    CHECK_TRUE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_FALSE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runFailed)
{
    CommandMock* subCommand = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->setFailed(true);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);

    CCcommand->run(true);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_TRUE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_FALSE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runPaused)
{
    CommandMock* subCommand = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->setPaused(true);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);

    CCcommand->run(true);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_TRUE(CCcommand->getPaused());
    CHECK_FALSE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runEmptySubcommands)
{
    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);

    CCcommand->run(true);

    CHECK_TRUE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_TRUE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runCommandNotStartedStarted)
{
    CommandMock* subCommand = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);
    mock("CommandComposite").setData("setStartedCommand", true);
    mock("CommandComposite").expectOneCall("startCommand").onObject(CCcommand).withParameter("attempt", true);
    mock("Command").expectOneCall("run").onObject(subCommand).withParameter("attempt", true);
    mock("Command").expectOneCall("isComplete").onObject(subCommand).andReturnValue(false);
    CCcommand->run(true);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_FALSE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runCommandNotStartedNotStarted)
{
    CommandMock* subCommand = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);
    mock("CommandComposite").setData("setStartedCommand", false);
    mock("CommandComposite").expectOneCall("startCommand").onObject(CCcommand).withParameter("attempt", false);

    CCcommand->run(false);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_FALSE(CCcommand->getSubCommands()->empty());
}
TEST(CommandComposite, runCommandSingleRun)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);
    mock("CommandComposite").setData("setStartedCommand", true);
    mock("CommandComposite").expectOneCall("startCommand").onObject(CCcommand).withParameter("attempt", false);
    mock("Command").expectOneCall("run").onObject(subCommand).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(subCommand).andReturnValue(false);

    CCcommand->run(false);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_EQUAL(2, CCcommand->getSubCommands()->size());
}
TEST(CommandComposite, runCommandFirstCompleted)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);

    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);
    mock("CommandComposite").setData("setStartedCommand", true);
    mock("CommandComposite").expectOneCall("startCommand").onObject(CCcommand).withParameter("attempt", false);
    mock("Command").expectOneCall("run").onObject(subCommand).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(subCommand).andReturnValue(true);
    mock("CommandComposite").expectOneCall("completedSubCommand").onObject(CCcommand);
    mock("CommandComposite").expectOneCall("updateStatus").onObject(CCcommand);
    mock("Command").expectOneCall("run").onObject(subCommand2).withParameter("attempt", false);
    mock("Command").expectOneCall("isComplete").onObject(subCommand2).andReturnValue(false);

    CCcommand->run(false);

    CHECK_FALSE(CCcommand->isComplete());
    CHECK_FALSE(CCcommand->hasFailed());
    CHECK_FALSE(CCcommand->getPaused());
    CHECK_EQUAL(1, CCcommand->getSubCommands()->size());
    CHECK_EQUAL(subCommand2, CCcommand->getSubCommands()->front());
}

TEST(CommandComposite, pauseAlreadyPaused)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);
    CCcommand->setPaused(true);

    CCcommand->pause();

    CHECK_TRUE(CCcommand->getPaused());
}
TEST(CommandComposite, pauseEmptySubCommands)
{
    CCcommand->pause();

    CHECK_TRUE(CCcommand->getPaused());
}
TEST(CommandComposite, pause)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);

    mock("Command").expectOneCall("pause").onObject(subCommand);

    CCcommand->pause();

    CHECK_TRUE(CCcommand->getPaused());
}
TEST(CommandComposite, resumeNotPaused)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);

    CCcommand->resume();

    CHECK_FALSE(CCcommand->getPaused());
}
TEST(CommandComposite, resumeEmptySubCommands)
{
    CCcommand->setPaused(true);

    CCcommand->resume();

    CHECK_FALSE(CCcommand->getPaused());
}
TEST(CommandComposite, resume)
{
    CommandMock* subCommand = new CommandMock();
    CommandMock* subCommand2 = new CommandMock();
    CCcommand->getSubCommands()->push(subCommand);
    CCcommand->getSubCommands()->push(subCommand2);
    CCcommand->setPaused(true);

    mock("Command").expectOneCall("resume").onObject(subCommand);

    CCcommand->resume();

    CHECK_FALSE(CCcommand->getPaused());
}
