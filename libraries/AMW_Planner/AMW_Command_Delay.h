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

class AMW_Command_Delay: public AMW_Command {
public:
    AMW_Command_Delay(uint32_t delay);

    void run(void);

    void pause(void);
    void resume(void);

private:
    uint32_t duration;
    uint32_t start;

    void updateStatus(void);
};

#endif /* AMW_COMMAND_DELAY_H_ */
