//
// Created by Suspense on 5/30/2023.
//

#ifndef AU_INPUTEVENT_H
#define AU_INPUTEVENT_H

namespace Audace {
	class InputEvent {
	public:
		const bool changed;
		const long changedTime;

		InputEvent(bool changed, long changedTime) : changed(changed), changedTime(changedTime) {}
	};
}

#endif //AU_INPUTEVENT_H
