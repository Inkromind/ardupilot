/*
 * AC_Behaviour.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_BEHAVIOUR_H_
#define AC_BEHAVIOUR_H_

class AC_Behaviour {

public:
    virtual bool isActive(void) = 0;
    virtual bool perform(void) = 0;

    virtual ~AC_Behaviour(void) {};

};

#endif /* MAD_BEHAVIOUR_H_ */
