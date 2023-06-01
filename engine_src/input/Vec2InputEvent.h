//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_VEC2INPUTEVENT_H
#define AU_VEC2INPUTEVENT_H

#include "InputEvent.h"
#include "glm/glm.hpp"

namespace Audace {
	class Vec2InputEvent : public InputEvent {
	public:
		const glm::vec2 state;

		Vec2InputEvent(glm::vec2 state, bool changed, long changedTime) :
		InputEvent(changed, changedTime), state(state) {}
	};
}

#endif //AU_VEC2INPUTEVENT_H
