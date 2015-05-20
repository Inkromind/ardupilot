/*
 * AMW_Task_RTL.cpp
 *
 *  Created on: 18-mei-2015
 *      Author: Arne
 */

#include "AMW_Task_RTL.h"
#include "AMW_Task_Planner.h"
#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Command_Nav_Assigned_Altitude.h"
#include "AMW_Command_Land.h"
#include "AMW_Command_Disarm.h"
#include <AC_Facade.h>
#include "AMW_Planner.h"

AMW_Task_RTL::AMW_Task_RTL() {
    taskId = 9;
}

AMW_Commands_Plan* AMW_Task_RTL::generatePlan() {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Takeoff_Assigned_Altitude());
    plan->addNewCommand(new AMW_Command_Nav_Assigned_Altitude(AMW_Task_Planner::getInstance()->getHomeBase()));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());
    return plan;
}

void AMW_Task_RTL::completeTask(void) {
#ifdef AMW_PLANNER_DEBUG
    AC_Facade::getFacade()->sendFormattedDebug(PSTR("Returned to home at <%.2f,%.2f>"), AMW_Task_Planner::getInstance()->getHomeBase().x, AMW_Task_Planner::getInstance()->getHomeBase().y);
#endif
}
