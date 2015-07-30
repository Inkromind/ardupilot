/*
 * AMW_Task_Test_Flight_2.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_TEST_FLIGHT_2_H_
#define AMW_TASK_TEST_FLIGHT_2_H_

#include "AMW_Task.h"

/**
 * Test flight 2
 */
class AMW_Task_Test_Flight_2: public AMW_Task {
public:
    AMW_Task_Test_Flight_2();

    AMW_Command* generatePlan(void) const;
};

#endif /* AMW_TASK_TEST_FLIGHT_2_H_ */
