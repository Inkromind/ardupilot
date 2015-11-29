/*
 * AMW_Task_RTL_Forced.cpp
 *
 *  Created on: 2-aug.-2015
 *      Author: Arne
 */

#include "AMW_Task_RTL_Forced.h"
#include "AMW_Commands_Plan.h"
#include "AMW_Command_Composite_Forced_Nav.h"
#include "AMW_Command_Disarm.h"
#include <AC_CommunicationFacade.h>
#include "AMW_Task_Planner.h"

AMW_Task_RTL_Forced::AMW_Task_RTL_Forced() {
    taskId = 8;
}

AMW_Command* AMW_Task_RTL_Forced::generatePlan(void) const {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Composite_Forced_Nav(AMW_Task_Planner::getInstance()->getHomeBase(), true));
    plan->addNewCommand(new AMW_Command_Disarm());
#ifdef AMW_TASK_DEBUG
    //AC_CommunicationFacade::sendDebug(PSTR("Generated plan to return home (forced)"));
#endif
    return plan;
}

void AMW_Task_RTL_Forced::completedTask(void) {
#ifdef AMW_TASK_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Returned to home at <%.2f,%.2f> (forced)"), AMW_Task_Planner::getInstance()->getHomeBase().x, AMW_Task_Planner::getInstance()->getHomeBase().y);
#endif
}
