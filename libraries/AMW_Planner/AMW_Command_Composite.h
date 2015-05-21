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
        while (!subCommands.empty()) {
            delete subCommands.front();
            subCommands.pop();
        }
    };

    virtual void run(void) {
        updateStatus();

        if (completed || failed || paused)
            return;
        if (subCommands.empty()) {
            completed = true;
            return;
        }
        subCommands.front()->run();
        commandStarted = true;
        if (subCommands.front()->isComplete()) {
            delete subCommands.front();
            subCommands.pop();
            run();
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

    virtual void updateStatus(void) = 0;

protected:
    AMW_Queue<AMW_Command*> subCommands;

};


#endif /* AMW_COMMAND_COMPOSITE_H_ */
