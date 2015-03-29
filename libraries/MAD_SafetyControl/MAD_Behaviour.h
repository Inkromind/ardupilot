/*
 * MAD_Behaviour.h
 *
 *  Created on: 30-mrt.-2015
 *      Author: Arne
 */

#ifndef MAD_BEHAVIOUR_H_
#define MAD_BEHAVIOUR_H_

class MAD_Behaviour {

public:
    virtual bool isActive(void) = 0;
    virtual bool perform(void) = 0;

    virtual ~MAD_Behaviour(void) {};

}

#endif /* MAD_BEHAVIOUR_H_ */
