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
        commandStarted = true;
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

    virtual void completedSubCommand() { }

    virtual void updateStatus(void) = 0;

    virtual void clearSubCommands() {
        while (!subCommands.empty()) {
            delete subCommands.front();
            subCommands.pop();
        }
    }

    virtual void startCommand(bool attempt) {
        commandStarted = true;
    }



protected:
    AMW_Queue<AMW_Command*> subCommands;

};


#endif /* AMW_COMMAND_COMPOSITE_H_ */
