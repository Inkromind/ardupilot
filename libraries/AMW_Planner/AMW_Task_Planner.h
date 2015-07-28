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

class AMW_Task_Planner {
public:

    virtual void run(void);
    virtual void init(void);

    void pauseMission(void);
    void toggleMission(void);
    void resumeMission(void);

    virtual AMW_Task* getFirstTask(bool* forceTask);
    virtual void completeFirstTask(AMW_Task* task);
    virtual void switchedToFirstTask(AMW_Task* task);
    virtual float addPackage(AMW_Task_Package* task, bool estimate);

    virtual void returnHome(void);

    float getAssignedAltitude(void) const {
        return assignedAltitude;
    }

    Vector2f getHomeBase(void) const {
        return homeBase;
    }

    static AMW_Task_Planner* getInstance(void);

    virtual void markBatteryEmpty(void);

    virtual bool isBatteryEmpty(void) const {
        return batteryEmpty;
    }

private:
    bool plannerInitialized;
    bool paused;
    bool returningHome;
    bool idleTaskCompleted;
    static AMW_Task_Planner* planner;
    AMW_List<AMW_Task*> plan;
    AMW_Task* idleTask;
    Vector2f homeBase;
    bool batteryEmpty;

    float assignedAltitude;

    AMW_Task_Planner();

    AMW_Task* getIdleTask(void);
protected:
    virtual ~AMW_Task_Planner();

};

#endif /* AMW_TASK_PLANNER_H_ */
