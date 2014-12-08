/*
 * MAS_ControlModule.h
 *
 *  Created on: 5-dec.-2014
 *      Author: Arne
 */

#ifndef MAS_CONTROLMODULE_H_
#define MAS_CONTROLMODULE_H_

class MAS_ControlModule
{
public:
    virtual boolean takeControl() = 0;

    virtual ~MAS_ControlModule() = 0;
};


#endif /* MAS_CONTROLMODULE_H_ */
