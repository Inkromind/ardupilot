/*
 * MAD_Safety_Control.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef MAD_SAFETY_CONTROL_H_
#define MAD_SAFETY_CONTROL_H_

#include "MAD_Behaviour.h"
#include <List>

class MAD_SafetyControl {

public:
    virtual void run(void);
    virtual void init(void);

    virtual bool isActive(void);

    static MAD_SafetyControl* getInstance(void);
private:
    std::list<MAD_Behaviour*>* behaviours;
    bool active;
    static MAD_SafetyControl* module;

    MAD_SafetyControl();
    virtual void startNewTask(void);
protected:
    virtual ~MAD_SafetyControl();
};

#endif /* MAD_SAFETY_CONTROL_H_ */
