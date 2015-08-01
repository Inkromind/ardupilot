/*
 * AMW_Task_Planner.h
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_TASK_PLANNER_H_
#define AMW_TASK_PLANNER_H_

#include "AMW_Task.h"
#include <AMW_List.h>
#include "../AP_Math/vector2.h"
#include "AMW_Task_Package.h"

#define AMW_PLANNER_BATTERY_TAKEOFF_LIMIT 30

/**
 * Schedules tasks
 */
class AMW_Task_Planner {
public:

    /**
     * Run the planner
     */
    virtual void run(void);

    /**
     * Initialize the planner
     */
    virtual void init(void);

    /**
     * Pause the mission
     */
    void pauseMission(void);

    /**
     * Toggle the mission
     */
    void toggleMission(void);

    /**
     * Resume the mission
     */
    void resumeMission(void);

    /**
     * Get the first task to execute
     *
     * @param forceTask (output) True if any current task must be discarded in
     * in favor for the given task (even if task is 0)
     * @return The task to perform or 0 if no more tasks to perform
     */
    virtual AMW_Task* getFirstTask(bool* forceTask);

    /**
     * Completed a task
     */
    virtual void completedTask(AMW_Task* task);

    /**
     * Switched to another task
     */
    virtual void switchedToTask(AMW_Task* task);

    /**
     * Add a task
     */
    virtual float addTask(AMW_Task* task, bool estimate);

    /**
     * Return home
     */
    virtual void returnHome(void);

    /**
     * Get the homebase (relative to origin)
     */
    Vector2f getHomeBase(void) const {
        return homeBase;
    }
    /**
     * Set the homebase (relative to origin)
     */
    void setHomeBase(Vector2f newBase) {
        homeBase = newBase;
    }

    static AMW_Task_Planner* getInstance(void);

    /**
     * Mark the battery as empty
     */
    virtual void markBatteryEmpty(void);

    /**
     * Check if the battery is marked as empty
     */
    virtual bool isBatteryEmpty(void) const {
        return batteryEmpty;
    }

private:
    bool plannerInitialized;
    Vector2f homeBase;
    bool batteryEmpty;

    /**
     * Get the task to perform if no more tasks are scheduled
     */
    AMW_Task* fetchIdleTask(void);
protected:
    bool paused;
    bool returningHome;
    bool idleTaskCompleted;
    AMW_List<AMW_Task*> plan;
    AMW_Task* idleTask;


    AMW_Task_Planner();
    virtual ~AMW_Task_Planner();
    static AMW_Task_Planner* planner;

};

#endif /* AMW_TASK_PLANNER_H_ */
