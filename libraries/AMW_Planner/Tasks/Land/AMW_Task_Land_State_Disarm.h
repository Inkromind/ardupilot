/*
 * AMW_Task_Land_State_Disarm.h
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_LAND_STATE_DISARM_H_
#define AMW_TASK_LAND_STATE_DISARM_H_

#include <AMW_Planner_Task_State.h>
#include <AMW_Planner_Task_State_With_Subtask.h>
#include <AMW_Task_Land.h>

class AMW_Task_Land_State_Disarm : public AMW_Planner_Task_State_With_Subtask {
public:
    AMW_Task_Land_State_Disarm(AMW_Task_Land& parentTask);

    AMW_Planner_Task_State& getNextState(void);
};

#endif /* AMW_TASK_LAND_STATE_DISARM_H_ */
