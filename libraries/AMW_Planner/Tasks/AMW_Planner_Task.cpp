/*
 * AMW_Planner_Task.cpp
 *
 *  Created on: 8-mrt.-2015
 *      Author: Arne
 */

#include "AMW_Planner_Task.h"

AMW_Planner_Task::~AMW_Planner_Task() {
    if (currentCommand)
        delete currentCommand;
}

bool AMW_Planner_Task::isComplete() {
    return this->completed;
}
