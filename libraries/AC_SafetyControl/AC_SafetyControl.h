/*
 * AC_SafetyControl.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_SAFETYCONTROL_H_
#define AC_SAFETYCONTROL_H_

#include "AC_Behaviour.h"
#include <AMW_List.h>

#define AC_SAFETYCONTROL_DEBUG

class AC_SafetyControl {

public:
    virtual void run(void);
    virtual void init(void);

    virtual bool isActive(void);

    static AC_SafetyControl* getInstance(void);
private:
    AMW_List<AC_Behaviour*> behaviours;
    bool active;
    static AC_SafetyControl* module;

    AC_SafetyControl();
protected:
    virtual ~AC_SafetyControl();
};

#endif /* AC_SAFETYCONTROL_H_ */
