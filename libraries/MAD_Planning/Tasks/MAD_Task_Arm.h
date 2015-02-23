/*
 * MAD_Task_Arm.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_ARM_H_
#define MAD_TASK_ARM_H_

class MAD_Task_Arm: public MAD_Task {
public:
    MAD_Task_Arm();
    virtual ~MAD_Task_Arm();

    bool isComplete();
    void runTask();
};

#endif /* MAD_TASK_ARM_H_ */
