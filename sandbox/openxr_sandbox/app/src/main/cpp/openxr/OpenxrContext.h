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
#include "input/BooleanInputEvent.h"
#include "input/PoseInputEvent.h"
#include "input/InputDevices.h"

class OpenxrContext {

public:
	XrInstance instance{XR_NULL_HANDLE};
	XrSystemId xrSystem{XR_NULL_SYSTEM_ID};
	XrSession xrSession{XR_NULL_HANDLE};
	XrSpace xrWorldSpace{XR_NULL_HANDLE};
	XrSpace xrViewSpace{XR_NULL_HANDLE};

	std::vector<OpenxrView> views;
	std::vector<XrViewConfigurationView> xrViewConfigs;
	uint32_t viewCount;

	std::map<Audace::OculusTouchController::InputName, XrAction> actions;
	std::map<Audace::OculusTouchController::InputName, std::function<void(Audace::BooleanInputEvent)>> booleanInputHandlers;
	std::map<Audace::OculusTouchController::InputName, std::function<void(Audace::PoseInputEvent)>> poseInputHandlers;

	XrPath leftHandPath{XR_NULL_PATH};
	XrPath rightHandPath{XR_NULL_PATH};
	XrActionSet actionSet{XR_NULL_HANDLE};
	XrSpace leftHandPoseSpace{XR_NULL_HANDLE};
	XrSpace rightHandPoseSpace{XR_NULL_HANDLE};
	XrSpace leftHandAimSpace{XR_NULL_HANDLE};

	bool init(android_app *app);

	bool createSession(EGLDisplay eglDisplay, EGLContext eglContext);

	bool beginSession();

	int64_t chooseViewFormat(XrSession session);

	bool registerActions();

	bool processActions(XrTime displayTime);

	void addBooleanInputHandler(Audace::OculusTouchController::InputName name,
								std::function<void(Audace::BooleanInputEvent)> handler) {
		booleanInputHandlers[name] = handler;
	}

	void addPoseInputHandler(Audace::OculusTouchController::InputName name,
								std::function<void(Audace::PoseInputEvent)> handler) {
		poseInputHandlers[name] = handler;
	}

	OpenxrView getView(uint32_t i) { return views[i]; }

private:
};


#endif //AUDACE_OPENXRCONTEXT_H
