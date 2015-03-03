/*
 * MAD_Task_Nav.h
 *
 *  Created on: 16-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_TASK_NAV_H_
#define MAD_TASK_NAV_H_

class MAD_Task_Nav: public MAD_Task {
public:
    MAD_Task_Nav(Vector3f destination);
    virtual ~MAD_Task_Nav();

    bool isComplete();
    void runTask();
private:
    Vector3f destination;
};

#endif /* MAD_TASK_NAV_H_ */
