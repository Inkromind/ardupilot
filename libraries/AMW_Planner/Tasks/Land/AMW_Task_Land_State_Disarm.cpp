/*
 * AMW_Task_Land_State_Disarm.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Land_State_Disarm.h"
#include <AMW_Task_State_Complete.h>
#include <AMW_Task_Disarm.h>

AMW_Task_Land_State_Disarm::AMW_Task_Land_State_Disarm(AMW_Task_Land& parentTask) {
    this->parentTask = parentTask;
    subTask = new AMW_Task_Disarm();
}

AMW_Task_State& AMW_Task_Land_State_Disarm::getNextState(void) {
    return new AMW_Task_State_Complete(parentTask);
}
