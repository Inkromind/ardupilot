/*
 * AMW_Command_Delay.h
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_COMMAND_DELAY_H_
#define AMW_COMMAND_DELAY_H_

#include "AMW_Command.h"
#include <stdint.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

/**
 * Make the sequencer wait a given amount of time
 */
class AMW_Command_Delay: public AMW_Command {
public:
    /**
     * @param delay The time to wait (in ms)
     */
    AMW_Command_Delay(uint32_t delay);

    void run(bool attempt);

    void pause(void);
    void resume(void);

private:
    uint32_t duration;
    uint32_t start;

    void updateStatus(void);
};

#endif /* AMW_COMMAND_DELAY_H_ */
