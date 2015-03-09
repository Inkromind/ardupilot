/*
 * AMW_Task_Command_Nav.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_COMMAND_NAV_H_
#define AMW_TASK_COMMAND_NAV_H_

class AMW_Task_Command_Nav: public AMW_Task_Command {
public:
    AMW_Task_Command_Nav(Vector3f destination);
    virtual ~AMW_Task_Command_Nav();

    void updateStatus();
    void runCommand();
private:
    Vector3f destination;
};

#endif /* AMW_TASK_COMMAND_NAV_H_ */
