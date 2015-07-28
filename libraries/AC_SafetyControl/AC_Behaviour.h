/*
 * AC_Behaviour.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef AC_BEHAVIOUR_H_
#define AC_BEHAVIOUR_H_

/**
 * Safety Control Behaviour module
 */
class AC_Behaviour {

public:
    /**
     * Check if the Behaviour Module is active
     *
     * @return True if this behaviour module is active. False otherwise
     */
    virtual bool isActive(void) = 0;

    /**
     * Run the behaviour module
     *
     * @return True if the module took control. False otherwise
     */
    virtual bool perform(void) = 0;

    virtual ~AC_Behaviour(void) {};

};

#endif /* MAD_BEHAVIOUR_H_ */
