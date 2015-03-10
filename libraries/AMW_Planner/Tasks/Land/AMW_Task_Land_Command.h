/*
 * AMW_Task_Land_Command.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_LAND_COMMAND_H_
#define AMW_TASK_LAND_COMMAND_H_

#include <AMW_Planner_Task.h>

class AMW_Task_Land_Command : public AMW_Planner_Task {
public:
    AMW_Task_Land_Command();

    void updateStatus(void);
    void runTask(void);
};

#endif /* AMW_TASK_LAND_COMMAND_H_ */
