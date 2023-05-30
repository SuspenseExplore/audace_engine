//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_BOOLEANACTIONEVENT_H
#define AU_BOOLEANACTIONEVENT_H

#include "InputEvent.h"

namespace Audace {
	class BooleanInputEvent : public InputEvent {
	public:
		const bool state;

		BooleanInputEvent(bool state, bool changed, long changedTime) :
		InputEvent(changed, changedTime), state(state) {}
	};
}

#endif //AU_BOOLEANACTIONEVENT_H
