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
    MAD_Task_Takeoff(float alt);
    virtual ~MAD_Task_Takeoff();

    bool isComplete();
    void runTask();

private:
    float altitude;
};

#endif /* MAD_TASK_TAKEOFF_H_ */
