//
// Created by Suspense on 3/4/2023.
//

#include <GLES3/gl31.h>
#include "OpenxrContext.h"
#include "xr_linear.h"

bool OpenxrContext::init(android_app *app) {
	PFN_xrInitializeLoaderKHR initializeLoaderFn = nullptr;
	if (XR_SUCCEEDED(
			xrGetInstanceProcAddr(nullptr, "xrInitializeLoaderKHR",
								  (PFN_xrVoidFunction *) (&initializeLoaderFn)))) {
		XrLoaderInitInfoAndroidKHR loaderInitInfoAndroid;
		memset(&loaderInitInfoAndroid, 0, sizeof(loaderInitInfoAndroid));
		loaderInitInfoAndroid.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
		loaderInitInfoAndroid.next = nullptr;
		loaderInitInfoAndroid.applicationVM = app->activity->vm;
		loaderInitInfoAndroid.applicationContext = app->activity->clazz;
		initializeLoaderFn((const XrLoaderInitInfoBaseHeaderKHR *) &loaderInitInfoAndroid);
	}

	if (instance == XR_NULL_HANDLE) {
		char n1[35] = "XR_KHR_android_create_instance";
		char n2[35] = "XR_KHR_opengl_es_enable";

		std::vector<char *> names;
		names.push_back(n1);
		names.push_back(n2);

		XrInstanceCreateInfoAndroidKHR instanceCreateInfoAndroid = {
				XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
		instanceCreateInfoAndroid.applicationVM = app->activity->vm;
		instanceCreateInfoAndroid.applicationActivity = app->activity->clazz;

		XrInstanceCreateInfo createInfo{XR_TYPE_INSTANCE_CREATE_INFO};
		createInfo.next = (void *) &instanceCreateInfoAndroid;
		createInfo.enabledExtensionCount = 2;
		createInfo.enabledExtensionNames = names.data();

		strcpy(createInfo.applicationInfo.applicationName, "openxr_intro");
		createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
		XrResult res = xrCreateInstance(&createInfo, &instance);
		if (res != XR_SUCCESS) {
			LOGE("xrCreateInstance failed: %d", res);
			return false;
		}
	}

	if (xrSystem == XR_NULL_SYSTEM_ID) {
		XrSystemGetInfo sysInfo{XR_TYPE_SYSTEM_GET_INFO};
		sysInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		XrResult res = xrGetSystem(instance, &sysInfo, &xrSystem);
		if (res != XR_SUCCESS) {
			LOGE("xrGetSystem failed: %d", res);
			return false;
		}

		PFN_xrGetOpenGLESGraphicsRequirementsKHR pfnGetOpenGLESGraphicsRequirementsKHR = nullptr;
		res = xrGetInstanceProcAddr(instance, "xrGetOpenGLESGraphicsRequirementsKHR",
									(PFN_xrVoidFunction *) (&pfnGetOpenGLESGraphicsRequirementsKHR));
		if (res != XR_SUCCESS) {
			LOGE("Failed to get function xrGetOpenGLESGraphicsRequirementsKHR: %d", res);
			return false;
		}

		XrGraphicsRequirementsOpenGLESKHR graphicsRequirements{
				XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR};
		res = pfnGetOpenGLESGraphicsRequirementsKHR(instance, xrSystem, &graphicsRequirements);
		if (res != XR_SUCCESS) {
			LOGE("xrGetOpenGLESGraphicsRequirementsKHR failed: %d", res);
			return false;
		}
	}

	Audace::OculusTouchController::init(instance);
	return true;
}

bool OpenxrContext::createSession(EGLDisplay eglDisplay, EGLContext eglContext) {
	if (xrSession == XR_NULL_HANDLE) {
		XrGraphicsBindingOpenGLESAndroidKHR binding{XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR};
		binding.display = eglDisplay;
		binding.context = eglContext;
		binding.config = (EGLConfig) 0;

		XrSessionCreateInfo sessionInfo{XR_TYPE_SESSION_CREATE_INFO};
		sessionInfo.next = reinterpret_cast<const XrBaseInStructure *>(&binding);
		sessionInfo.systemId = xrSystem;
		XrResult res = xrCreateSession(instance, &sessionInfo, &xrSession);
		if (res != XR_SUCCESS) {
			LOGE("xrCreateSessionFailed: %d", res);
			return false;
		}
	}

	XrResult res = xrEnumerateViewConfigurationViews(instance, xrSystem,
													 XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
													 0, &viewCount, nullptr);
	if (res != XR_SUCCESS) {
		LOGE("xrEnumerateViewConfigurationViews (getting count) failed: %d", res);
		return false;
	}

	xrViewConfigs.resize(viewCount, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
	res = xrEnumerateViewConfigurationViews(instance, xrSystem,
											XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
											viewCount,
											&viewCount, xrViewConfigs.data());
	if (res != XR_SUCCESS) {
		LOGE("xrEnumerateViewConfigurationViews (getting list) failed: %d", res);
		return false;
	}
	views.resize(viewCount);

	int64_t chosenFormat = chooseViewFormat(xrSession);
	for (uint32_t j = 0; j < viewCount; j++) {
		const XrViewConfigurationView &cfg = xrViewConfigs[j];

		if (!views[j].init(xrSession, cfg, chosenFormat)) {
			return false;
		}
	}

	return true;
}

bool OpenxrContext::beginSession() {
	XrSessionBeginInfo sessionBeginInfo{XR_TYPE_SESSION_BEGIN_INFO};
	sessionBeginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	XR_ERROR_BAIL("xrBeginSession", xrBeginSession(xrSession, &sessionBeginInfo));

	XrPosef pose;
	pose.position.x = pose.position.y = pose.position.z = 0;
	XrVector3f axis{1.0f, 0.0f, 0.0f};
	XrQuaternionf_CreateFromAxisAngle(&pose.orientation, &axis, MATH_PI / -2.0f);
	XrReferenceSpaceCreateInfo referenceSpaceCreateInfo{
			XR_TYPE_REFERENCE_SPACE_CREATE_INFO};

	referenceSpaceCreateInfo.poseInReferenceSpace = pose;
	referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
	XR_ERROR_BAIL("xrCreateReferenceSpace",
				  xrCreateReferenceSpace(xrSession, &referenceSpaceCreateInfo,
										 &xrSpace));
	return true;
}

int64_t OpenxrContext::chooseViewFormat(XrSession session) {
	uint32_t formatCount;
	XrResult res = xrEnumerateSwapchainFormats(session, 0, &formatCount, nullptr);
	if (res != XR_SUCCESS) {
		LOGE("xrEnumerateSwapchainFormats (getting count) failed: %d", res);
		return -1;
	}
	std::vector<int64_t> formats(formatCount);
	res = xrEnumerateSwapchainFormats(session, (uint32_t) formats.size(), &formatCount,
									  formats.data());
	if (res != XR_SUCCESS) {
		LOGE("xrEnumerateSwapchainFormats (getting list) failed: %d", res);
		return -1;
	}
	std::vector<int64_t> supportedFormats{GL_RGBA8, GL_RGBA8_SNORM, GL_SRGB8_ALPHA8};
	return *std::find_first_of(formats.begin(), formats.end(),
							   supportedFormats.begin(),
							   supportedFormats.end());

}

bool OpenxrContext::registerActions() {
	std::vector<XrActionSuggestedBinding> bindings{};

	XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
	strcpy(actionSetInfo.actionSetName, "main_scene");
	strcpy(actionSetInfo.localizedActionSetName, "Main Scene");
	actionSetInfo.priority = 0;
	XR_ERROR_BAIL("xrCreateActionSet",
				  xrCreateActionSet(instance, &actionSetInfo, &actionSet));
	XR_ERROR_BAIL("xrStringToPath", xrStringToPath(instance, "/user/hand/left", &leftHandPath));

	{
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_GRIP_POSE;
		XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
		actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
		strcpy(actionInfo.actionName, "left_hand_pose");
		strcpy(actionInfo.localizedActionName, "Left hand pose");
		actionInfo.countSubactionPaths = 1;
		actionInfo.subactionPaths = &leftHandPath;

		XrAction action;
		XR_LOG_ERROR("xrCreateAction",
					  xrCreateAction(actionSet, &actionInfo, &action));
		actions[name] = action;
		bindings.push_back(XrActionSuggestedBinding{action, Audace::OculusTouchController::paths[name]});
	}
	{
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_AIM_POSE;
		XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
		actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
		strcpy(actionInfo.actionName, "left_hand_aim");
		strcpy(actionInfo.localizedActionName, "Left hand aim");
		actionInfo.countSubactionPaths = 1;
		actionInfo.subactionPaths = &leftHandPath;

		XrAction action;
		XR_LOG_ERROR("xrCreateAction",
					 xrCreateAction(actionSet, &actionInfo, &action));
		actions[name] = action;
		bindings.push_back(XrActionSuggestedBinding{action, Audace::OculusTouchController::paths[name]});
	}

	{
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_X_CLICK;
		XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
		actionInfo.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
		strcpy(actionInfo.actionName, "x_button_click");
		strcpy(actionInfo.localizedActionName, "X Button Click");
		actionInfo.countSubactionPaths = 1;
		actionInfo.subactionPaths = &leftHandPath;

		XrAction action;
		XR_LOG_ERROR("xrCreateAction",
					 xrCreateAction(actionSet, &actionInfo, &action));
		actions[name] = action;
		bindings.push_back(
				XrActionSuggestedBinding{action, Audace::OculusTouchController::paths[name]});
	}
	{
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_Y_CLICK;
		XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
		actionInfo.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
		strcpy(actionInfo.actionName, "y_button_click");
		strcpy(actionInfo.localizedActionName, "Y Button Click");
		actionInfo.countSubactionPaths = 1;
		actionInfo.subactionPaths = &leftHandPath;

		XrAction action;
		XR_LOG_ERROR("xrCreateAction",
					 xrCreateAction(actionSet, &actionInfo, &action));
		actions[name] = action;
		bindings.push_back(
				XrActionSuggestedBinding{action, Audace::OculusTouchController::paths[name]});
	}

	XrPath touchControllerPath;
	XR_ERROR_BAIL("xrStringToPath",
				  xrStringToPath(instance, "/interaction_profiles/oculus/touch_controller",
								 &touchControllerPath));

	XrInteractionProfileSuggestedBinding suggestedBinding{
			XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
	suggestedBinding.interactionProfile = touchControllerPath;
	suggestedBinding.suggestedBindings = bindings.data();
	suggestedBinding.countSuggestedBindings = (uint32_t) bindings.size();
	XR_ERROR_BAIL("xrSuggestInteractionProfileBindings",
				  xrSuggestInteractionProfileBindings(instance, &suggestedBinding));

	{
		XrActionSpaceCreateInfo actionSpaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
		actionSpaceInfo.action = actions[Audace::OculusTouchController::LEFT_GRIP_POSE];
		actionSpaceInfo.poseInActionSpace.orientation.w = 1;
		actionSpaceInfo.subactionPath = leftHandPath;
		XR_ERROR_BAIL("xrCreateActionSpace",
					  xrCreateActionSpace(xrSession, &actionSpaceInfo, &leftHandPoseSpace));
	}
	{
		XrActionSpaceCreateInfo actionSpaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
		actionSpaceInfo.action = actions[Audace::OculusTouchController::LEFT_AIM_POSE];
		actionSpaceInfo.poseInActionSpace.orientation.w = 1;
		actionSpaceInfo.subactionPath = leftHandPath;
		XR_ERROR_BAIL("xrCreateActionSpace",
					  xrCreateActionSpace(xrSession, &actionSpaceInfo, &leftHandAimSpace));
	}
	XrSessionActionSetsAttachInfo attachInfo{XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = &actionSet;
	XR_ERROR_BAIL("xrAttachSessionActionSets", xrAttachSessionActionSets(xrSession, &attachInfo));

	return true;
}

bool OpenxrContext::processActions(XrTime displayTime) {
	XrActiveActionSet activeActionSet{actionSet, XR_NULL_PATH};
	XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO};
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = &activeActionSet;
	xrSyncActions(xrSession, &syncInfo);

	if (poseInputHandlers.find(Audace::OculusTouchController::LEFT_GRIP_POSE) != poseInputHandlers.end()) {
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_GRIP_POSE;
		XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
		getInfo.action = actions[name];

		XrActionStatePose poseState{XR_TYPE_ACTION_STATE_POSE};
		XR_ERROR_BAIL("xrGetActionStatePose",
					  xrGetActionStatePose(xrSession, &getInfo, &poseState));
		if (poseState.isActive == XR_TRUE && poseInputHandlers.find(name) != poseInputHandlers.end()) {
			XrSpaceLocation location{XR_TYPE_SPACE_LOCATION};
			xrLocateSpace(leftHandPoseSpace, xrSpace, displayTime, &location);
			Audace::Pose pose;
			pose.position = glm::vec3(location.pose.position.x, location.pose.position.y, location.pose.position.z);
			pose.orientation = glm::quat(location.pose.orientation.w, location.pose.orientation.x, location.pose.orientation.y, location.pose.orientation.z);
			poseInputHandlers[name](Audace::PoseInputEvent(pose, true, displayTime));
		}
	}
	if (poseInputHandlers.find(Audace::OculusTouchController::LEFT_AIM_POSE) != poseInputHandlers.end()) {
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_AIM_POSE;
		XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
		getInfo.action = actions[name];

		XrActionStatePose poseState{XR_TYPE_ACTION_STATE_POSE};
		XR_ERROR_BAIL("xrGetActionStatePose",
					  xrGetActionStatePose(xrSession, &getInfo, &poseState));
		if (poseState.isActive == XR_TRUE && poseInputHandlers.find(name) != poseInputHandlers.end()) {
			XrSpaceLocation location{XR_TYPE_SPACE_LOCATION};
			xrLocateSpace(leftHandAimSpace, xrSpace, displayTime, &location);
			Audace::Pose pose;
			pose.position = glm::vec3(location.pose.position.x, location.pose.position.y, location.pose.position.z);
			pose.orientation = glm::quat(location.pose.orientation.w, location.pose.orientation.x, location.pose.orientation.y, location.pose.orientation.z);
			poseInputHandlers[name](Audace::PoseInputEvent(pose, true, displayTime));
		}
	}
	if (booleanInputHandlers.find(Audace::OculusTouchController::LEFT_X_CLICK) != booleanInputHandlers.end()) {
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_X_CLICK;
		std::function<void(Audace::BooleanInputEvent)> handler = booleanInputHandlers[name];
		XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
		getInfo.action = actions[name];

		XrActionStateBoolean booleanState{XR_TYPE_ACTION_STATE_BOOLEAN};
		XR_LOG_ERROR("xrGetActionStateBoolean",
					 xrGetActionStateBoolean(xrSession, &getInfo, &booleanState));

		AU_OPENXR_LOG_TRACE("Action state: {}; {}", name, booleanState.currentState);
		handler(Audace::BooleanInputEvent(booleanState.currentState,
										  booleanState.changedSinceLastSync,
										  booleanState.lastChangeTime));
	}
	if (booleanInputHandlers.find(Audace::OculusTouchController::LEFT_Y_CLICK) != booleanInputHandlers.end()) {
		Audace::OculusTouchController::InputName name = Audace::OculusTouchController::LEFT_Y_CLICK;
		std::function<void(Audace::BooleanInputEvent)> handler = booleanInputHandlers[name];
		XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
		getInfo.action = actions[name];

		XrActionStateBoolean booleanState{XR_TYPE_ACTION_STATE_BOOLEAN};
		XR_LOG_ERROR("xrGetActionStateBoolean",
					 xrGetActionStateBoolean(xrSession, &getInfo, &booleanState));

		AU_OPENXR_LOG_TRACE("Action state: {}; {}", name, booleanState.currentState);
		handler(Audace::BooleanInputEvent(booleanState.currentState,
										  booleanState.changedSinceLastSync,
										  booleanState.lastChangeTime));
	}
	return true;
}