/*
 * TaskPlannerMock.h
 *
 *  Created on: 30-jul.-2015
 *      Author: Arne
 */

#ifndef MOCKS_SEQUENCERMOCK_H_
#define MOCKS_SEQUENCERMOCK_H_

#include "../../libraries/AMW_Planner/AMW_Sequencer.h"

class SequencerMock : public AMW_Sequencer {
public:
    static void setSequencer(SequencerMock* sequencer) {
        AMW_Sequencer::sequencer = sequencer;
    }
    static void deleteSequencer() {
        AMW_Sequencer::sequencer = 0;
    }
};



#endif /* MOCKS_SEQUENCERMOCK_H_ */
