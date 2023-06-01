//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_BUTTONINPUTEVENT_H
#define AU_BUTTONINPUTEVENT_H

#include "InputEvent.h"

namespace Audace {
	class ButtonInputEvent : public InputEvent {
	public:
		int key;
		bool pressed;

		ButtonInputEvent(int key, bool pressed, bool changed, long changedTime) :
		InputEvent(changed, changedTime), key(key), pressed(pressed) {}
	};
}

#endif //AU_BUTTONINPUTEVENT_H
