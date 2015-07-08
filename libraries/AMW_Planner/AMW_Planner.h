/*
 * AMW_Planner.h
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_H_
#define AMW_PLANNER_H_

#ifndef TESTENV
    #define AMW_PLANNER_DEBUG
#endif

#include "AMW_Task_Planner.h"

class AMW_Planner {
public:
    static void initPlanner(void);

    static void run100Hz();

    static void run50Hz();

    static void run10Hz();

    static void run1Hz();

    static void pauseMission(void);

    static void resumeMission(void);

    static void returnHome(void);

private:
    AMW_Planner();
    ~AMW_Planner();

    static void toggleMission(void);
    static void checkMissionToggle(void);

    static bool previousToggleState;
    static bool initialized;

};

#endif /* AMW_PLANNER_H_ */
