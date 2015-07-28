/*
 * AC_Behaviour_Battery_Monitor.h
 *
 * Check if the battery level is below a safety limit.
 * If this is detected, the module will take control and
 * land automatically.
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_BEHAVIOUR_BATTERY_MONITOR_H_
#define AC_BEHAVIOUR_BATTERY_MONITOR_H_

#include "AC_Behaviour.h"
#include <AMW_Corridors.h>
#include <AMW_List.h>

// Battery Limit. If battery percentage drops below this, the drone will land
#define AC_BATTERY_LIMIT 25

class AC_Behaviour_Battery_Monitor: public AC_Behaviour {
public:
    AC_Behaviour_Battery_Monitor() {
        activated = false;
        landCorridor = 0;
    };
    virtual ~AC_Behaviour_Battery_Monitor() {
        if (landCorridor) {
            AMW_List<AMW_Corridor*> corridors;
            corridors.push_back(landCorridor);
            AMW_Corridor_Manager::getInstance()->freeCorridors(&corridors);
        }
    };

    /**
     * Run the behaviour module.
     * If the module is active, it will try to land and disarm motors.
     *
     * @return True if the module took control. False otherwise
     */
    bool perform(void);

    /**
     * Check if the Behaviour Module is active
     *
     * @return True if the battery level is below the limit. False otherwise.
     */
    bool isActive(void);

private:
    /** Has the module been activated     */
    bool activated;
    AMW_Corridor* landCorridor;

};

#endif /* AC_BEHAVIOUR_BATTERY_MONITOR_H_ */
