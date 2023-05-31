//
// Created by Suspense on 5/30/2023.
//

#ifndef AU_INPUTDEVICES_H
#define AU_INPUTDEVICES_H


#include "openxr/openxr_common.h"

namespace Audace {

	class OculusTouchController {
	public:
		enum InputName {
			LEFT_X_CLICK, LEFT_X_TOUCH, LEFT_Y_CLICK, LEFT_Y_TOUCH,
			LEFT_MENU_CLICK, LEFT_SQUEEZE_VALUE, LEFT_TRIGGER_VALUE, LEFT_TRIGGER_TOUCH,
			LEFT_THUMBSTICK_X, LEFT_THUMBSTICK_Y, LEFT_THUMBSTICK_CLICK, LEFT_THUMBSTICK_TOUCH,
			LEFT_THUMBREST_TOUCH, LEFT_GRIP_POSE, LEFT_AIM_POSE,
			RIGHT_A_CLICK, RIGHT_A_TOUCH, RIGHT_B_CLICK, RIGHT_B_TOUCH,
			RIGHT_SYSTEM_CLICK, RIGHT_SQUEEZE_VALUE, RIGHT_TRIGGER_VALUE, RIGHT_TRIGGER_TOUCH,
			RIGHT_THUMBSTICK_X, RIGHT_THUMBSTICK_Y, RIGHT_THUMBSTICK_CLICK, RIGHT_THUMBSTICK_TOUCH,
			RIGHT_THUMBREST_TOUCH, RIGHT_GRIP_POSE, RIGHT_AIM_POSE,
		};

		static const std::string leftHandString;
		static const std::string rightHandString;
		static std::string stringPaths[30];
		static XrPath paths[30];

		static void init(XrInstance instance);
	};

	class InputDevices {
	public:

		static XrPath leftHandPath;
		static XrPath rightHandPath;
		static XrPath oculusTouchPaths[30];
		static void init(XrInstance instance);
	};
}

#endif //AU_INPUTDEVICES_H
