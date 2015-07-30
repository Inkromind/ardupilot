/*
 * AMW_Command_Composite.h
 *
 *  Created on: 21-mei-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_COMPOSITE_H_
#define AMW_COMMAND_COMPOSITE_H_

#include "AMW_Command.h"
#include <AMW_Queue.h>

/**
 * A command consisting of one or more subcommands
 */
class AMW_Command_Composite: public AMW_Command {
public:
    AMW_Command_Composite() : AMW_Command() {}

    virtual ~AMW_Command_Composite() {
        clearSubCommands();
    };

    virtual void run(bool attempt) {
        updateStatus();

        if (completed || failed || paused)
            return;
        if (subCommands.empty()) {
            completed = true;
            return;
        }
        if (!commandStarted) {
            startCommand(attempt);
            if (!commandStarted)
                return;
        }
        subCommands.front()->run(attempt);
        if (subCommands.front()->isComplete()) {
            delete subCommands.front();
            subCommands.pop();
            completedSubCommand();
            run(attempt);
        }
    }

    virtual void pause() {
        if (paused)
            return;
        paused = true;
        if (!subCommands.empty())
            subCommands.front()->pause();
    };

    virtual void resume() {
        if (!paused)
           return;
        paused = false;
        if (!subCommands.empty())
            subCommands.front()->resume();
    };

protected:
    /**
     * A subcommand has completed
     */
    virtual void completedSubCommand() { }

    /**
     * Update the status of the command
     */
    virtual void updateStatus(void) = 0;

    /**
     * Clear all subcommands
     */
    virtual void clearSubCommands() {
        while (!subCommands.empty()) {
            delete subCommands.front();
            subCommands.pop();
        }
    }

    /**
     * Start the command
     *
     * @param attempt Set to true to only do a single attempt
     * at starting the command and fail right away.
     */
    virtual void startCommand(bool attempt) {
        commandStarted = true;
    }
    AMW_Queue<AMW_Command*> subCommands;

};


#endif /* AMW_COMMAND_COMPOSITE_H_ */
