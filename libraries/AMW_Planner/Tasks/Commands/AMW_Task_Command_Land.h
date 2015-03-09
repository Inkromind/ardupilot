/*
 * AMW_Task_Command_Land.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_COMMAND_LAND_H_
#define AMW_TASK_COMMAND_LAND_H_

class AMW_Task_Command_Land: public AMW_Task_Command {
public:
    AMW_Task_Command_Land();
    virtual ~AMW_Task_Command_Land();

    void updateStatus();
    void runCommand();
};

#endif /* AMW_TASK_COMMAND_LAND_H_ */
