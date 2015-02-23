/*
 * MAD_Task_Land.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_LAND_H_
#define MAD_TASK_LAND_H_

class MAD_Task_Land: public MAD_Task {
public:
    MAD_Task_Land();
    virtual ~MAD_Task_Land();

    bool isComplete();
    void runTask();
};

#endif /* MAD_TASK_LAND_H_ */
