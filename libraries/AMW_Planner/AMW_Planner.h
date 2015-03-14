/*
 * AMW_Planner.h
 *
 *  Created on: 13-mrt.-2015
 *      Author: Arne
 */

#ifndef AMW_PLANNER_H_
#define AMW_PLANNER_H_

class AMW_Planner {
public:
    static void initPlanner(void);

    static void run100Hz();

    static void run50Hz();

    static void run10Hz();

    static void run1Hz();

private:
    AMW_Planner();
    ~AMW_Planner();

};

#endif /* AMW_PLANNER_H_ */
