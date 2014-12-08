/*
 * MAS_MissionPlanner.cpp
 *
 *  Created on: 5-dec.-2014
 *      Author: Arne
 */

#include "MAS_MissionPlanner.h"

MAS_MissionPlanner::MAS_MissionPlanner()
{
    // Add Tasks
}

MAS_MissionPlanner::takeControl()
{
    MAS_MissionTask *currentTask = mission.front();

    currentTask->runTask();

    if (currentTask->isCompleted())
        mission.pop();

    return true;

}
