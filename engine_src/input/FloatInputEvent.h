//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_FLOATINPUTEVENT_H
#define AU_FLOATINPUTEVENT_H

#include "InputEvent.h"

namespace Audace {
	class FloatInputEvent : public InputEvent {
	public:
		const float state;

		FloatInputEvent(float state, bool changed, long changedTime) :
		InputEvent(changed, changedTime), state(state) {}
	};
}

#endif //AU_FLOATINPUTEVENT_H
