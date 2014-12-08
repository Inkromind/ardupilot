/*
 * MAS_MissionPlanner.h
 *
 *  Created on: 5-dec.-2014
 *      Author: Arne
 */

#ifndef MAS_MISSIONPLANNER_H_
#define MAS_MISSIONPLANNER_H_

#include <queue>
#include "MAS_ControlModule.h"
#include "MAS_MissionTask.h"

class MAS_MissionPlanner : public MAS_ControlModule {
public:
    MAS_MissionPlanner();

    boolean takeControl();

private:
    std::queue<*MissionTask> mission;
};



#endif /* MAS_MISSIONPLANNER_H_ */
