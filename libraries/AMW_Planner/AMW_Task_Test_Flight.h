/*
 * AMW_Task_Test_Flight.h
 *
 *  Created on: 12-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_TEST_FLIGHT_H_
#define AMW_TASK_TEST_FLIGHT_H_

#include "AMW_Task.h"

/**
 * Test flight 1
 */
class AMW_Task_Test_Flight: public AMW_Task {
public:
    AMW_Task_Test_Flight();

    AMW_Command* generatePlan(void) const;
};

#endif /* AMW_TASK_TEST_FLIGHT_H_ */
