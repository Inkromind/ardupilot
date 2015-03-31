/*
 * AC_Behaviour_Battery_Monitor.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_BEHAVIOUR_BATTERY_MONITOR_H_
#define AC_BEHAVIOUR_BATTERY_MONITOR_H_

#include "AC_Behaviour.h"

#define AC_BATTERY_LIMIT 50

class AC_Behaviour_Battery_Monitor: public AC_Behaviour {
public:
    AC_Behaviour_Battery_Monitor() {
        activated = false;
    };
    virtual ~AC_Behaviour_Battery_Monitor() {};

    bool perform(void);
    bool isActive(void);

private:
    bool activated;
};

#endif /* AC_BEHAVIOUR_BATTERY_MONITOR_H_ */
