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

/**
 * A Reactive Module for the safety of the drone
 * This module can override the long term planner if necessary,
 * e.g. the remaining battery level drops below a critical level.
 */
class AC_SafetyControl {

public:
    /**
     * Run the SafetyControl Module
     */
    virtual void run(void);

    /**
     * Initialize the SafetyControl Module
     */
    virtual void init(void);

    /**
     * Returns if the SafetyControl Module is taking control
     *
     * @return True if the module is taking control. False otherwise
     */
    virtual bool isActive(void) const;

    /**
     * Get the module instance
     */
    static AC_SafetyControl* getInstance(void);

    /**
     * Get the identifier for this module
     */
    static AMW_Module_Identifier* getModuleIdentifier(void) {
          return moduleIdentifier;
    }
private:
    bool active;
    static AC_SafetyControl* module;

    static AMW_Module_Identifier* moduleIdentifier;
protected:
    AMW_List<AC_Behaviour*> behaviours;
    AC_SafetyControl();
    virtual ~AC_SafetyControl();
};

#endif /* AC_SAFETYCONTROL_H_ */
