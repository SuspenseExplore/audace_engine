//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_KEYINPUTEVENT_H
#define AU_KEYINPUTEVENT_H

#include "InputEvent.h"

namespace Audace {
	class KeyInputEvent : public InputEvent {
	public:
		int key;
		bool pressed;

		KeyInputEvent(int key, bool pressed, bool changed, long changedTime) :
		InputEvent(changed, changedTime), key(key), pressed(pressed) {}
	};
}

#endif //AU_KEYINPUTEVENT_H
