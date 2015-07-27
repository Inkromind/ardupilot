/*
 * AMW_Task_Package.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Task_Package.h"
#include "AMW_Command_Composite_Nav_Assigned_Altitude.h"
#include "AMW_Command_Delay.h"
#include "AMW_Command_Disarm.h"
#include "AMW_Commands_Plan.h"
#include <AC_CommunicationFacade.h>
#include "AMW_Planner.h"

AMW_Task_Package::AMW_Task_Package(uint8_t newId, Vector2f newPickupLocation, Vector2f newDeliveryLocation) {
    this->id = newId;
    this->pickupLocation = newPickupLocation;
    this->deliveryLocation = newDeliveryLocation;
    this->taskId = newId * 10 + 1;
}

AMW_Command* AMW_Task_Package::generatePlan(void) {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Composite_Nav_Assigned_Altitude(pickupLocation));
    plan->addNewCommand(new AMW_Command_Disarm());
    plan->addNewCommand(new AMW_Command_Delay(15000));
    plan->addNewCommand(new AMW_Command_Composite_Nav_Assigned_Altitude(deliveryLocation));
    plan->addNewCommand(new AMW_Command_Disarm());
    plan->addNewCommand(new AMW_Command_Delay(15000));
#ifdef AMW_TASK_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Generated plan for package #%d"), id);
#endif
    return plan;
}

void AMW_Task_Package::completeTask(void) {
#ifdef AMW_TASK_DEBUG
    AC_CommunicationFacade::sendFormattedDebug(PSTR("Completed package #%d"), id);
#endif
}

