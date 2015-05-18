/*
 * AMW_Task_Package.cpp
 *
 *  Created on: 13-mei-2015
 *      Author: Arne
 */

#include "AMW_Task_Package.h"
#include "AMW_Command_Nav_Assigned_Altitude.h"
#include "AMW_Command_Takeoff_Assigned_Altitude.h"
#include "AMW_Command_Delay.h"
#include "AMW_Command_Land.h"
#include "AMW_Command_Disarm.h"
#include "AMW_Commands_Plan.h"

AMW_Task_Package::AMW_Task_Package(uint8_t newId, Vector2f newPickupLocation, Vector2f newDeliveryLocation) {
    this->id = newId;
    this->pickupLocation = newPickupLocation;
    this->deliveryLocation = newDeliveryLocation;
}

AMW_Commands_Plan* AMW_Task_Package::generatePlan(void) {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();
    plan->addNewCommand(new AMW_Command_Takeoff_Assigned_Altitude());
    plan->addNewCommand(new AMW_Command_Nav_Assigned_Altitude(pickupLocation));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());
    plan->addNewCommand(new AMW_Command_Delay(30));
    plan->addNewCommand(new AMW_Command_Takeoff_Assigned_Altitude());
    plan->addNewCommand(new AMW_Command_Nav_Assigned_Altitude(deliveryLocation));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());
    plan->addNewCommand(new AMW_Command_Delay(30));

    return plan;
}

