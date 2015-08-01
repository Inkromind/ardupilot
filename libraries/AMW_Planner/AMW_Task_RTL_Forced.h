/*
 * AMW_Task_RTL_Forced.h
 *
 *  Created on: 2-aug.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_RTL_FORCED_H_
#define AMW_TASK_RTL_FORCED_H_

#include "AMW_Task_RTL.h"

class AMW_Task_RTL_Forced: public AMW_Task_RTL {
public:
    AMW_Task_RTL_Forced();

    virtual AMW_Command* generatePlan(void) const;

    virtual void completeTask(void);
};

#endif /* AMW_TASK_RTL_FORCED_H_ */
