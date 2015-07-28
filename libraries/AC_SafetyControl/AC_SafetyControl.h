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
#include <AMW_Modules.h>

#ifndef TESTENV
    //#define AC_SAFETYCONTROL_DEBUG
#endif

class AC_SafetyControl {

public:
    virtual void run(void);
    virtual void init(void);

    virtual bool isActive(void) const;

    static AC_SafetyControl* getInstance(void);

    static AMW_Module_Identifier* getModuleIdentifier(void) {
          return moduleIdentifier;
    }
private:
    bool active;
    static AC_SafetyControl* module;

    static AMW_Module_Identifier* moduleIdentifier;
    AC_SafetyControl();
protected:
    AMW_List<AC_Behaviour*> behaviours;
    virtual ~AC_SafetyControl();
};

#endif /* AC_SAFETYCONTROL_H_ */
