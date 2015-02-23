/*
 * MAD_Task_Takeoff.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_TAKEOFF_H_
#define MAD_TASK_TAKEOFF_H_

#include "MAD_Task.h"

class MAD_Task_Takeoff: public MAD_Task {
public:
    MAD_Task_Takeoff(uint8_t alt);
    virtual ~MAD_Task_Takeoff();

    bool isComplete();
    void runTask();

private:
    uint8_t altitude;
};

#endif /* MAD_TASK_TAKEOFF_H_ */
