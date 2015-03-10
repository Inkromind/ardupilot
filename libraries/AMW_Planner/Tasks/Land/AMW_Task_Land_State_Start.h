/*
 * AMW_Task_Land_Start_State.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_LAND_STATE_START_H_
#define AMW_TASK_LAND_STATE_START_H_

#include <AMW_Task_State.h>
#include <AMW_Task_State_With_Subtask.h>

class AMW_Task_Land_State_Start : public AMW_Task_State_With_Subtask {
public:
    AMW_Task_Land_State_Start(AMW_Task_Land& parentTask);

    AMW_Task_State& getNextState(void);
};

#endif /* AMW_TASK_LAND_STATE_START_H_ */
