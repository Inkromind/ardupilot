/*
 * AMW_Command.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_H_
#define AMW_COMMAND_H_

#define AMW_COMMAND_DEBUG

class AMW_Command {
public:
    AMW_Command() {
        this->commandStarted = false;
        this->completed = false;
        this->failed = false;
        this->paused = false;
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

protected:
    bool commandStarted;
    bool completed;
    bool failed;
    bool paused;
};

#endif /* AMW_COMMAND_H_ */
