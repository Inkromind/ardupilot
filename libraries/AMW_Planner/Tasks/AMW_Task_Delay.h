/*
 * AMW_Task_Delay.h
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_DELAY_H_
#define AMW_TASK_DELAY_H_

class AMW_Task_Delay: public AMW_Planner_Task {
public:
    AMW_Task_Delay(uint32_t delay);
    virtual ~AMW_Task_Delay();

    void runTask(void);
    void updateStatus(void);

private:
    uint32_t duration;
    uint32_t start;
};

#endif /* AMW_TASK_DELAY_H_ */
