/*
 * MAS_Delegator.cpp
 *
 *  Created on: 5-dec.-2014
 *      Author: Arne
 */

#include "MAS_Delegator.h"

MAS_Delegator::MAS_Delegator()
{
    modules.push_back(new MAS_MissionPlanner);
}

MAS_Delegator::run()
{
    for (auto module = modules.begin(); module != modules.end(); ++module)
    {
        if (module->takeControl())
        {
            return;
        }
    }
}
