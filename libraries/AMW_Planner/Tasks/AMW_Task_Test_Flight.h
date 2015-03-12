/*
 * AMW_Task_Test_Flight.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_TEST_FLIGHT_H_
#define AMW_TASK_TEST_FLIGHT_H_

#include <AMW_Commands_Plan.h>

class AMW_Task_Test_Flight: public AMW_Task {
public:
    virtual AMW_Task_Test_Flight();

    AMW_Commands_Plan* generatePlan(void);
};

#endif /* AMW_TASK_TEST_FLIGHT_H_ */
