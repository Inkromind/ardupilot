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
#include <AP_Math.h>

class AMW_Task_Planner {
public:

    virtual void run(void);
    virtual void init(void);

    void pauseMission(void);
    void toggleMission(void);
    void resumeMission(void);

    virtual AMW_Task* getFirstTask(void);
    virtual void completeFirstTask(void);
    virtual void addTask(AMW_Task* task);
    virtual float addPackage(AMW_Task_Package* task, bool estimate = false);

    float getAssignedAltitude(void) {
        return assignedAltitude;
    }

    static AMW_Task_Planner* getInstance(void);
private:
    bool plannerInitialized;
    bool paused;
    static AMW_Task_Planner* planner;
    AMW_List<AMW_Task*> plan;
    Vector3f homeBase;

    float assignedAltitude;

    AMW_Task_Planner();
protected:
    virtual ~AMW_Task_Planner();

};

#endif /* AMW_TASK_PLANNER_H_ */
