//
// Created by Suspense on 5/30/2023.
//

#ifndef AU_POSEINPUTEVENT_H
#define AU_POSEINPUTEVENT_H

#include "InputEvent.h"
#include "math/Pose.h"

namespace Audace {
	class PoseInputEvent : public InputEvent {
	public:
		const Pose state;

		PoseInputEvent(Pose state, bool changed, long changedTime) :
				InputEvent(changed, changedTime), state(state) {}
	};
}

#endif //AU_POSEINPUTEVENT_H
