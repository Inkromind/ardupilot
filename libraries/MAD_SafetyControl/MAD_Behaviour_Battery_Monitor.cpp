/*
 * MAD_Behaviour_Battery_Monitor.cpp
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#include "MAD_Behaviour_Battery_Monitor.h"
#include <AP_BattMonitor.h>

bool MAD_Behaviour_Battery_Monitor::isActive(void) {
    if (activated)
        return true;

    return (AC_Facade::getBattery()->capacity_remaining_pct() < MAD_BATTERY_LIMIT);
}

bool MAD_Behaviour_Battery_Monitor::perform() {
    return true;
}
