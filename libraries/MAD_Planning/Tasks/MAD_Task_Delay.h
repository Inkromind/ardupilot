/*
 * MAD_Task_Delay.h
 *
 *  Created on: 20-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_DELAY_H_
#define MAD_TASK_DELAY_H_

class MAD_Task_Delay: public MAD_Task {
public:
    MAD_Task_Delay(uint32_t delay);
    virtual ~MAD_Task_Delay();

    bool isComplete();
    void runTask();

private:
    uint32_t duration;
    uint32_t start;
};

#endif /* MAD_TASK_DELAY_H_ */
