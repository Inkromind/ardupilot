/*
 * AMW_Task_Land_State_Start.cpp
 *
 *  Created on: 10-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Land_State_Start.h"
#include "AMW_Task_Land_Command.h"
#include "AMW_Task_Land_State_Disarm.h"

AMW_Task_Land_State_Start::AMW_Task_Land_State_Start(AMW_Task_Land& parentTask) {
    this->parentTask = parentTask;
    subTask = new AMW_Task_Land_Command();
}

AMW_Task_State& AMW_Task_Land_State_Start::getNextState(void) {
    return new AMW_Task_Land_State_Disarm((AMW_Task_Land) parentTask);
}

