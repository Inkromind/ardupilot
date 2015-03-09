/*
 * AMW_Task_Command_Delay.h
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_COMMAND_DELAY_H_
#define AMW_TASK_COMMAND_DELAY_H_

class AMW_Task_Command_Delay: public AMW_Task_Command {
public:
    AMW_Task_Command_Delay(uint32_t delay);

    void runCommand(void);
    void updateStatus(void);

private:
    uint32_t duration;
    uint32_t start;
};

#endif /* AMW_TASK_COMMAND_DELAY_H_ */
