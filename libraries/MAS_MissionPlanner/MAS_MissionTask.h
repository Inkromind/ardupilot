/*
 * MAS_MissionTask.h
 *
 *  Created on: 8-dec.-2014
 *      Author: Arne
 */

#ifndef MAS_MISSIONTASK_H_
#define MAS_MISSIONTASK_H_

class MAS_MissionTask {
    virtual void runTask() = 0;

    virtual boolean isCompleted() = 0;

    virtual void ~MAS_MissionTask();
};

#endif /* MAS_MISSIONTASK_H_ */
