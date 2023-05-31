//
// Created by Suspense on 5/30/2023.
//

#include <string>
#include "InputDevices.h"

namespace Audace {
	void InputDevices::init(XrInstance instance) {
		OculusTouchController::init(instance);
	}

	const std::string OculusTouchController::leftHandString = "/user/hand/left";
	const std::string OculusTouchController::rightHandString = "/user/hand/right";
	std::string OculusTouchController::stringPaths[30];
	XrPath OculusTouchController::paths[30];

	void OculusTouchController::init(XrInstance instance) {
		stringPaths[LEFT_X_CLICK] = leftHandString + "/input/x/click";
		stringPaths[LEFT_X_TOUCH] = leftHandString + "/input/x/touch";
		stringPaths[LEFT_Y_CLICK] = leftHandString + "/input/y/click";
		stringPaths[LEFT_Y_TOUCH] = leftHandString + "/input/y/touch";
		stringPaths[LEFT_MENU_CLICK] = leftHandString + "/input/menu/click";
		stringPaths[LEFT_SQUEEZE_VALUE] = leftHandString + "/input/squeeze/value";
		stringPaths[LEFT_TRIGGER_VALUE] = leftHandString + "/input/trigger/value";
		stringPaths[LEFT_TRIGGER_TOUCH] = leftHandString + "/input/trigger/touch";
		stringPaths[LEFT_THUMBSTICK_X] = leftHandString + "/input/thumbstick/x";
		stringPaths[LEFT_THUMBSTICK_Y] = leftHandString + "/input/thumbstick/y";
		stringPaths[LEFT_THUMBSTICK_CLICK] = leftHandString + "/input/thumbstick/click";
		stringPaths[LEFT_THUMBSTICK_TOUCH] = leftHandString + "/input/thumbstick/touch";
		stringPaths[LEFT_THUMBREST_TOUCH] = leftHandString + "/input/thumbrest/touch";
		stringPaths[LEFT_GRIP_POSE] = leftHandString + "/input/grip/pose";
		stringPaths[LEFT_AIM_POSE] = leftHandString + "/input/aim/pose";

		stringPaths[RIGHT_A_CLICK] = rightHandString + "/input/a/click";
		stringPaths[RIGHT_A_TOUCH] = rightHandString + "/input/a/touch";
		stringPaths[RIGHT_B_CLICK] = rightHandString + "/input/b/click";
		stringPaths[RIGHT_B_TOUCH] = rightHandString + "/input/b/touch";
		stringPaths[RIGHT_SYSTEM_CLICK] = rightHandString + "/input/system/click";
		stringPaths[RIGHT_SQUEEZE_VALUE] = rightHandString + "/input/squeeze/value";
		stringPaths[RIGHT_TRIGGER_VALUE] = rightHandString + "/input/trigger/value";
		stringPaths[RIGHT_TRIGGER_TOUCH] = rightHandString + "/input/trigger/touch";
		stringPaths[RIGHT_THUMBSTICK_X] = rightHandString + "/input/thumbstick/x";
		stringPaths[RIGHT_THUMBSTICK_Y] = rightHandString + "/input/thumbstick/y";
		stringPaths[RIGHT_THUMBSTICK_CLICK] = rightHandString + "/input/thumbstick/click";
		stringPaths[RIGHT_THUMBSTICK_TOUCH] = rightHandString + "/input/thumbstick/touch";
		stringPaths[RIGHT_THUMBREST_TOUCH] = rightHandString + "/input/thumbrest/touch";
		stringPaths[RIGHT_GRIP_POSE] = rightHandString + "/input/grip/pose";
		stringPaths[RIGHT_AIM_POSE] = rightHandString + "/input/aim/pose";

		for (int i = 0; i < 30; i++) {
			xrStringToPath(instance, stringPaths[i].c_str(), &paths[i]);
		}
	}
}