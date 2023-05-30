//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_BOOLEANACTIONEVENT_H
#define AU_BOOLEANACTIONEVENT_H

namespace Audace {
	class BooleanInputEvent {
	public:
		const bool state;
		const bool changed;
		const long changedTime;

		BooleanInputEvent(bool state, bool changed, long changedTime) :
		state(state), changed(changed), changedTime(changedTime) {}
	};
}

#endif //AU_BOOLEANACTIONEVENT_H
