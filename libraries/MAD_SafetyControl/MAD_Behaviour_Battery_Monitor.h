/*
 * MAD_Behaviour_Battery_Monitor.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef MAD_BEHAVIOUR_BATTERY_MONITOR_H_
#define MAD_BEHAVIOUR_BATTERY_MONITOR_H_

#include "MAD_Behaviour.h"

#define MAD_BATTERY_LIMIT 50

class MAD_Behaviour_Battery_Monitor: public MAD_Behaviour {
public:
    MAD_Behaviour_Battery_Monitor() {
        activated = false;
    };
    virtual ~MAD_Behaviour_Battery_Monitor() {};

    bool perform(void);
    bool isActive(void);

private:
    bool activated;
};

#endif /* MAD_BEHAVIOUR_BATTERY_MONITOR_H_ */
