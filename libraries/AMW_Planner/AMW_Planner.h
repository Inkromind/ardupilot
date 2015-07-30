/*
 * AMW_Planner.h
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_H_
#define AMW_PLANNER_H_

#ifndef TESTENV
//    #define AMW_PLANNER_DEBUG
#endif

#include "AMW_Task_Planner.h"
#include <AMW_Modules.h>

/**
 * Facade to the Planner Module
 */
class AMW_Planner {
public:
    /**
     * Initialize the planner module
     */
    static void initPlanner(void);

    static void run100Hz();

    static void run50Hz();

    static void run10Hz();

    static void run1Hz();

    /**
     * Pause the current mission
     */
    static void pauseMission(void);

    /**
     * Resume the current mission
     */
    static void resumeMission(void);

    /**
     * Return and land at the home location
     */
    static void returnHome(void);

    /**
     * Check if the battery is empty
     */
    static bool isBatteryEmpty(void);

    /**
     * Mark the battery as empty
     */
    static void markBatteryEmpty(void);

    /**
     * Get the identifier for the Plannermodule
     */
    static AMW_Module_Identifier* getModuleIdentifier(void) {
          return moduleIdentifier;
    }

private:
    AMW_Planner();
    ~AMW_Planner();

    /**
     * Toggle the mission state (paused)
     */
    static void toggleMission(void);

    /**
     * Check if the mission state (paused) should be toggled
     */
    static void checkMissionToggle(void);

    static bool previousToggleState;
    static bool initialized;

    static AMW_Module_Identifier* moduleIdentifier;

};

#endif /* AMW_PLANNER_H_ */
