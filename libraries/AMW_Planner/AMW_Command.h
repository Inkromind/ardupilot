/*
 * AMW_Command.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_H_
#define AMW_COMMAND_H_

class AMW_Command {
public:
    virtual ~AMW_Command() {};

    virtual void updateStatus(void) = 0;

    virtual void runCommand(void) = 0;

    virtual bool isComplete(void) {
        return completed;
    }

    virtual void pauseCommand() {};
    virtual void resumeCommand() {};

protected:
    bool commandStarted;
    bool completed;
};

#endif /* AMW_COMMAND_H_ */
