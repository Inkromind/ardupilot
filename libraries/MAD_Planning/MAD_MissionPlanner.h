/*
 * MAD_MissionPlanner.h
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#ifndef MAD_MISSIONPLANNER_H_
#define MAD_MISSIONPLANNER_H_

#include <queue>

class MAD_MissionPlanner {
public:
    static virtual MAD_MissionPlanner* getInstance(void);
    virtual void run();
    virtual void init();

private:
    static MAD_MissionPlanner planner;

    bool initialized = false;

    MAD_Task* currentTask = 0;

    std::queue<*MAD_Task> taskQueue;

    MAD_MissionPlanner();
    virtual ~MAD_MissionPlanner();

    void startNextTask();
};


#endif /* MAD_MISSIONPLANNER_H_ */
