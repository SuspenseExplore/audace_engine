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

	XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
	strcpy(actionSetInfo.actionSetName, "main_scene");
	strcpy(actionSetInfo.localizedActionSetName, "Main Scene");
	actionSetInfo.priority = 0;
	XR_ERROR_BAIL("xrCreateActionSet", xrCreateActionSet(instance, &actionSetInfo, &mainSceneActionSet));
	XR_ERROR_BAIL("xrStringToPath", xrStringToPath(instance, "/user/hand/left", &leftHandPath));

	// create a pose input action for the left controller
	XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
	actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
	strcpy(actionInfo.actionName, "left_hand_pose");
	strcpy(actionInfo.localizedActionName, "Left hand pose");
	actionInfo.countSubactionPaths = 1;
	actionInfo.subactionPaths = &leftHandPath;
	XR_ERROR_BAIL("xrCreateAction", xrCreateAction(mainSceneActionSet, &actionInfo, &leftHandPoseAction));

	XrPath leftPosePath;
	XR_ERROR_BAIL("xrStringToPath", xrStringToPath(instance, "/user/hand/left/input/grip/pose", &leftPosePath));

	XrPath touchControllerPath;
	XR_ERROR_BAIL("xrStringToPath", xrStringToPath(instance, "/interaction_profiles/oculus/touch_controller", &touchControllerPath));
	std::vector<XrActionSuggestedBinding> bindings{{{leftHandPoseAction, leftPosePath}}};
	XrInteractionProfileSuggestedBinding suggestedBinding{XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
	suggestedBinding.interactionProfile = touchControllerPath;
	suggestedBinding.suggestedBindings = bindings.data();
	suggestedBinding.countSuggestedBindings = (uint32_t) bindings.size();
	XR_ERROR_BAIL("xrSuggestInteractionProfileBindings", xrSuggestInteractionProfileBindings(instance, &suggestedBinding));

	XrActionSpaceCreateInfo actionSpaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
	actionSpaceInfo.action = leftHandPoseAction;
	actionSpaceInfo.poseInActionSpace.orientation.w = 1;
	actionSpaceInfo.subactionPath = leftHandPath;
	XR_ERROR_BAIL("xrCreateActionSpace", xrCreateActionSpace(xrSession, &actionSpaceInfo, &leftHandSpace));

	XrSessionActionSetsAttachInfo attachInfo{XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = &mainSceneActionSet;
	XR_ERROR_BAIL("xrAttachSessionActionSets", xrAttachSessionActionSets(xrSession, &attachInfo));

	return true;
}

bool OpenxrContext::processActions(XrTime displayTime) {
	XrActiveActionSet activeActionSet{mainSceneActionSet, XR_NULL_PATH};
	XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO};
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = &activeActionSet;
	xrSyncActions(xrSession, &syncInfo);

	XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
	getInfo.action = leftHandPoseAction;

	XrActionStatePose poseState{XR_TYPE_ACTION_STATE_POSE};
	XR_ERROR_BAIL("xrGetActionStatePose", xrGetActionStatePose(xrSession, &getInfo, &poseState));
	if (poseState.isActive == XR_TRUE) {
		xrLocateSpace(leftHandSpace, xrSpace, displayTime, &leftHandLocation);
	}
	return true;
}