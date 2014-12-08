/*
 * MAS_Delegator.h
 *
 *  Created on: 5-dec.-2014
 *      Author: Arne
 */

#ifndef MAS_DELEGATOR_H_
#define MAS_DELEGATOR_H_

#include <vector>
#include "MAS_ControlModule.h"

class MAS_Delegator {
public:
    MAS_Delegator();

    void run();

private:
    std::vector<MAS_ControlModule*> modules;
};

#endif /* MAS_DELEGATOR_H_ */
