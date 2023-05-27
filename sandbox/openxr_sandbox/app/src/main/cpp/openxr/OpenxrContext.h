//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_OPENXRCONTEXT_H
#define AUDACE_OPENXRCONTEXT_H

#include <vector>

#include "openxr_common.h"
#include "android_native_app_glue.h"

#include "OpenxrView.h"

class OpenxrContext {

public:
	XrInstance instance{XR_NULL_HANDLE};
	XrSystemId xrSystem{XR_NULL_SYSTEM_ID};
	XrSession xrSession{XR_NULL_HANDLE};
	XrSpace xrSpace{XR_NULL_HANDLE};

	std::vector<OpenxrView> views;
	std::vector<XrViewConfigurationView> xrViewConfigs;
	uint32_t viewCount;

	XrPath leftHandPath{XR_NULL_PATH};
	XrActionSet mainSceneActionSet{XR_NULL_HANDLE};
	XrAction leftHandPoseAction{XR_NULL_HANDLE};
	XrSpace leftHandSpace{XR_NULL_HANDLE};
	XrSpaceLocation leftHandLocation{XR_TYPE_SPACE_LOCATION};

	bool init(android_app* app);
	bool createSession(EGLDisplay eglDisplay, EGLContext eglContext);
	bool beginSession();
	int64_t chooseViewFormat(XrSession session);

	bool registerActions();
	bool processActions(XrTime displayTime);

	OpenxrView getView(uint32_t i) {return views[i];}
};


#endif //AUDACE_OPENXRCONTEXT_H
