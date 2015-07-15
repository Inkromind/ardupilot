/*
 * AMW_Command.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_H_
#define AMW_COMMAND_H_

#ifndef TESTENV
    //#define AMW_COMMAND_DEBUG
#endif

#include <AMW_Corridors.h>
#include <AMW_List.h>

class AMW_Command {
public:
    AMW_Command() {
        this->commandStarted = false;
        this->completed = false;
        this->failed = false;
        this->paused = false;
        this->parentCommand = 0;
    }
    virtual ~AMW_Command() {};

    virtual void run(void) = 0;

    virtual bool isComplete(void) {
        return completed;
    }

    virtual void pause(void) {};
    virtual void resume(void) {};

    virtual bool hasFailed(void) {
        return failed;
    }

    virtual void setParentCommand(AMW_Command* parent) {
        parentCommand = parent;
    }

    virtual AMW_List<AMW_Corridor*>* getCorridors(void) {
        return 0;
    }

protected:
    bool commandStarted;
    bool completed;
    bool failed;
    bool paused;

    AMW_Command* parentCommand;
};

#endif /* AMW_COMMAND_H_ */
