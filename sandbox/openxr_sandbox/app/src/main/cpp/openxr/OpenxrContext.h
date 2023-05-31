//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_OPENXRCONTEXT_H
#define AUDACE_OPENXRCONTEXT_H

#include <vector>
#include <map>
#include <functional>

#include "openxr_common.h"
#include "android_native_app_glue.h"

#include "OpenxrView.h"
#include "input/BooleanInputHandler.h"
#include "input/InputDevices.h"

class OpenxrContext {

public:
	XrInstance instance{XR_NULL_HANDLE};
	XrSystemId xrSystem{XR_NULL_SYSTEM_ID};
	XrSession xrSession{XR_NULL_HANDLE};
	XrSpace xrSpace{XR_NULL_HANDLE};

	std::vector<OpenxrView> views;
	std::vector<XrViewConfigurationView> xrViewConfigs;
	uint32_t viewCount;

//	std::vector<Audace::BooleanInputHandler*> booleanInputHandlers;
	std::map<Audace::OculusTouchController::InputName, XrAction> actions;
	std::map<Audace::OculusTouchController::InputName, std::function<void(Audace::BooleanInputEvent)>> booleanInputHandlers;

	XrPath leftHandPath{XR_NULL_PATH};
	XrActionSet actionSet{XR_NULL_HANDLE};
	XrAction leftHandPoseAction{XR_NULL_HANDLE};
	XrSpace leftHandSpace{XR_NULL_HANDLE};
	XrSpaceLocation leftHandLocation{XR_TYPE_SPACE_LOCATION};

	bool init(android_app *app);

	bool createSession(EGLDisplay eglDisplay, EGLContext eglContext);

	bool beginSession();

	int64_t chooseViewFormat(XrSession session);

	bool registerActions();

	bool processActions(XrTime displayTime);

	void addBooleanInputHandler(Audace::OculusTouchController::InputName name,
								std::function<void(Audace::BooleanInputEvent)> handler) {
//		booleanInputHandlers.push_back(handler);
		booleanInputHandlers[name] = handler;
	}

	OpenxrView getView(uint32_t i) { return views[i]; }

private:
};


#endif //AUDACE_OPENXRCONTEXT_H
