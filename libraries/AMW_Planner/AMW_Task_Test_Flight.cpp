/*
 * AMW_Task_Test_Flight.cpp
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Task_Test_Flight.h"
#include "AMW_Command_Takeoff.h"
#include "AMW_Command_Delay.h"
#include "AMW_Command_Nav.h"
#include "AMW_Command_Nav_Altitude.h"
#include "AMW_Command_Land.h"
#include "AMW_Command_Disarm.h"
#include "AMW_Commands_Plan.h"


AMW_Task_Test_Flight::AMW_Task_Test_Flight() { }

AMW_Command* AMW_Task_Test_Flight::generatePlan(void) const {
    AMW_Commands_Plan* plan = new AMW_Commands_Plan();

    plan->addNewCommand(new AMW_Command_Takeoff(2000));
    plan->addNewCommand(new AMW_Command_Delay(20000));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(5000, 0, 2000)));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(5000, 5000, 2000)));
    plan->addNewCommand(new AMW_Command_Nav_Altitude(1000));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(0, 5000, 1000)));
    plan->addNewCommand(new AMW_Command_Nav_Altitude(4500));
    plan->addNewCommand(new AMW_Command_Nav(Vector3f(0, 0, 4500)));
    plan->addNewCommand(new AMW_Command_Land());
    plan->addNewCommand(new AMW_Command_Disarm());

    return plan;
}
