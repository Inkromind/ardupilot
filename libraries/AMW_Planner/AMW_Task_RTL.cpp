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

AMW_Task_RTL::AMW_Task_RTL() { }

AMW_Commands_Plan* AMW_Task_RTL::generatePlan() {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Takeoff_Assigned_Altitude());
    plan->addNewCommand(new AMW_Command_Nav_Assigned_Altitude(AMW_Task_Planner::getInstance()->getHomeBase()));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());
    return plan;
}

