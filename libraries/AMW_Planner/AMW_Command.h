/*
 * AMW_Command.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_H_
#define AMW_COMMAND_H_

#ifndef TESTENV
    #define AMW_COMMAND_DEBUG
#endif

#include <AMW_Corridors.h>
#include <AMW_List.h>

/**
 * A (short-term) goal to complete.
 */
class AMW_Command {
public:
    AMW_Command() {
        this->commandStarted = false;
        this->completed = false;
        this->failed = false;
        this->paused = false;
    }
    virtual ~AMW_Command() {};

    /**
     * Execute the command
     *
     * @param attempt Set to true to only do a single attempt
     * at starting the command and fail right away.
     */
    virtual void run(bool attempt) = 0;

    /**
     * Check if the command has completed
     */
    virtual bool isComplete(void) const {
        return completed;
    }

    /**
     * Check if the command succesfully started
     */
    virtual bool hasStarted(void) const {
        return commandStarted;
    }

    /**
     * Pause the command
     */
    virtual void pause(void) {};

    /**
     * Resume the command
     */
    virtual void resume(void) {};

    /**
     * Check if the command has failed
     */
    virtual bool hasFailed(void) const {
        return failed;
    }

protected:
    bool commandStarted;
    bool completed;
    bool failed;
    bool paused;
};

#endif /* AMW_COMMAND_H_ */
