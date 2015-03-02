/*
 * MAD_Facade.cpp
 *
 *  Created on: 10-feb.-2015
 *      Author: Arne
 */

#include "MAD_Facade.h"
#include "MAD_MissionPlanner.h"

void MAD_Facade::initPlanner() {
    MAD_MissionPlanner::getInstance()->init();
}
