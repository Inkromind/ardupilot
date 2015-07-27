/*
 * AMW_Task_RTL.cpp
 *
 *  Created on: 18-mei-2015
 *      Author: Arne
 */

#include "AMW_Task_RTL.h"
#include "AMW_Task_Planner.h"
#include "AMW_Command_Composite_Nav_Assigned_Altitude.h"
#include "AMW_Command_Disarm.h"
#include <AC_CommunicationFacade.h>
#include "AMW_Commands_Plan.h"

AMW_Task_RTL::AMW_Task_RTL() {
    taskId = 9;
}

AMW_Command* AMW_Task_RTL::generatePlan() {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Composite_Nav_Assigned_Altitude(AMW_Task_Planner::getInstance()->getHomeBase()));
    plan->addNewCommand(new AMW_Command_Disarm());
#ifdef AMW_TASK_DEBUG
    AC_CommunicationFacade::sendDebug(PSTR("Generated plan to return home"));
#endif
    return plan;
}

void AMW_Task_RTL::completeTask(void) {
#ifdef AMW_TASK_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Returned to home at <%.2f,%.2f>"), AMW_Task_Planner::getInstance()->getHomeBase().x, AMW_Task_Planner::getInstance()->getHomeBase().y);
#endif
}
