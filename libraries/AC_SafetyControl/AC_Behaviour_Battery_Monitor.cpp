/*
 * AC_Behaviour_Battery_Monitor.cpp
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#include "AC_Behaviour_Battery_Monitor.h"
#include <AP_BattMonitor.h>
#include <AC_ReactiveFacade.h>
#include "AC_SafetyControl.h"

bool AC_Behaviour_Battery_Monitor::isActive(void) {
    if (activated)
        return true;
    if (AC_ReactiveFacade::getReactiveFacade()->getBattery()->capacity_remaining_pct() < AC_BATTERY_LIMIT) {
#ifdef AC_SAFETYCONTROL_DEBUG
        AC_ReactiveFacade::getFacade()->sendDebug(PSTR("Battery Low! Landing..."));
#endif
        activated = true;
    }
    return activated;
}

bool AC_Behaviour_Battery_Monitor::perform() {
    if (activated) {
        if (AC_ReactiveFacade::getReactiveFacade()->isLanded()) {
            if (AC_ReactiveFacade::getReactiveFacade()->areMotorsArmed())
                return AC_ReactiveFacade::getReactiveFacade()->disarmMotors();
            else
                return true;
        }
        return AC_ReactiveFacade::getReactiveFacade()->land();
    }
    return false;
}
