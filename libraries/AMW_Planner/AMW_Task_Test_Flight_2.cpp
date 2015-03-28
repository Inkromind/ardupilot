/*
 * AMW_Task_Test_Flight_2.cpp
 *
 *  Created on: 28-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Test_Flight_2.h"
#include "AMW_Command_Takeoff.h"
#include "AMW_Command_Delay.h"
#include "AMW_Command_Nav.h"
#include "AMW_Command_Nav_Altitude.h"
#include "AMW_Command_Land.h"
#include "AMW_Command_Disarm.h"

AMW_Task_Test_Flight_2::AMW_Task_Test_Flight_2() { }

AMW_Commands_Plan* AMW_Task_Test_Flight_2::generatePlan(void) {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();

    plan->addNewCommand(new AMW_Command_Takeoff(1500));
    plan->addNewCommand(new AMW_Command_Delay(10000));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(5000, 0, 1500)));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(5000, -5000, 1500)));
    plan->addNewCommand(new AMW_Command_Nav_Altitude(500));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(0, -5000, 500)));
    plan->addNewCommand(new AMW_Command_Nav_Altitude(3500));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(0, 0, 3500)));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());

    return plan;
}

