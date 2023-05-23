//
// Created by Suspense on 3/4/2023.
//

#include <GLES3/gl32.h>

#include "AppController.h"

bool AppController::createWindow() {
	return window.init(androidApp);
}

bool AppController::init(android_app *app) {
	androidApp = app;
	return xrContext.init(androidApp);
}

bool AppController::createXrSession() {
	glGenFramebuffers(1, &framebuffer);
	scene.init(androidApp->activity->assetManager);
	return xrContext.createSession(window.getDisplay(), window.getContext());
}

XrFrameState *AppController::startFrame() {
	if (xrContext.xrSession == XR_NULL_HANDLE) {
		LOGD("Skipping frame render; session is null");
		return nullptr;
	}

	XrResult res;
	XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
	currentFrameState = {XR_TYPE_FRAME_STATE};
	res = xrWaitFrame(xrContext.xrSession, &frameWaitInfo, &currentFrameState);
	if (res != XR_SUCCESS) {
		LOGE("xrWaitFrame failed: %d", res);
		return nullptr;
	}

	XrFrameBeginInfo frameBeginInfo{XR_TYPE_FRAME_BEGIN_INFO};
	res = xrBeginFrame(xrContext.xrSession, &frameBeginInfo);
	if (res != XR_SUCCESS) {
		LOGE("xrBeginFrame failed: %d", res);
		return nullptr;
	}

	return &currentFrameState;
}

bool AppController::endFrame(std::vector<XrCompositionLayerBaseHeader *> layers) {
	XrFrameEndInfo frameEndInfo{XR_TYPE_FRAME_END_INFO};
	frameEndInfo.displayTime = currentFrameState.predictedDisplayTime;
	frameEndInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	frameEndInfo.layerCount = (uint32_t) layers.size();
	frameEndInfo.layers = layers.data();
	XR_ERROR_BAIL("xrEndFrame", xrEndFrame(xrContext.xrSession, &frameEndInfo));

	return true;
}

uint32_t AppController::prepareViews(XrFrameState *frameState) {
	XrViewState viewState{XR_TYPE_VIEW_STATE};
	auto viewCapacityInput = (uint32_t) xrContext.views.size();
	uint32_t viewCountOutput;

	XrViewLocateInfo viewLocateInfo{XR_TYPE_VIEW_LOCATE_INFO};
	viewLocateInfo.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	viewLocateInfo.displayTime = frameState->predictedDisplayTime;
	viewLocateInfo.space = xrContext.xrSpace;

	std::vector<XrView> views;
	views.resize(xrContext.views.size(), {XR_TYPE_VIEW});
	XrResult res = xrLocateViews(xrContext.xrSession, &viewLocateInfo, &viewState,
								 viewCapacityInput, &viewCountOutput,
								 views.data());
	if (res != XR_SUCCESS) {
		LOGE("xrLocateViews failed: %d", res);
		return 0;
	}

	if ((viewState.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
		(viewState.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
		return 0;  // There is no valid tracking poses for the views.
	}

	if (viewCountOutput != viewCapacityInput) {
		LOGE("xrLocateViews != viewCapacityInput.  %d != %d", viewCountOutput, viewCapacityInput);
		return 0;
	}
	if (viewCountOutput != xrContext.xrViewConfigs.size()) {
		LOGE("xrLocateViews != xrViewConfigs.size().  %d != %d", viewCountOutput,
			 (int) xrContext.xrViewConfigs.size());
		return 0;
	}
	if (viewCountOutput != xrContext.views.size()) {
		LOGE("xrLocateViews != auViews.size().  %d != %d", viewCountOutput,
			 (int) xrContext.views.size());
		return 0;
	}
	for (int i = 0; i < views.size(); i++) {
		xrContext.views[i].setViewData(views[i]);
	}

	return viewCountOutput;
}

void AppController::renderFrame() {
	XrFrameState *frameState = startFrame();

	if (frameState != nullptr) {
		std::vector<XrCompositionLayerBaseHeader *> layers;
		XrCompositionLayerProjection layer{XR_TYPE_COMPOSITION_LAYER_PROJECTION};

		std::vector<XrCompositionLayerProjectionView> projectionLayerViews;
		if (frameState->shouldRender == XR_TRUE) {
			uint32_t viewCount = prepareViews(frameState);
			if (viewCount > 0) {
				projectionLayerViews.resize(viewCount);
				if (renderLayer(projectionLayerViews, layer)) {
					layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&layer));
				}
			}
		}
		endFrame(layers);
	}
}

bool AppController::renderLayer(std::vector<XrCompositionLayerProjectionView> &projectionLayerViews,
								XrCompositionLayerProjection &layer) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	XrResult res;

	// Render view to the appropriate part of the swapchain image.
	for (uint32_t i = 0; i < projectionLayerViews.size(); i++) {
		// Each view has a separate swapchain which is acquired, rendered to, and released.
		OpenxrView view = xrContext.getView(i);
		OpenxrSwapchain swapchain = view.getSwapchain();
		if (!swapchain.startFrame()) {
			return false;
		}

		projectionLayerViews[i] = {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW};
		projectionLayerViews[i].pose = view.getViewData().pose;
		projectionLayerViews[i].fov = view.getViewData().fov;
		projectionLayerViews[i].subImage.swapchain = swapchain.getHandle();
		projectionLayerViews[i].subImage.imageRect.offset = {0, 0};
		projectionLayerViews[i].subImage.imageRect.extent = {view.getWidth(), view.getHeight()};

		renderView(view);
		if (!swapchain.endFrame()) {
			return false;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	layer.space = xrContext.xrSpace;
	layer.layerFlags = 0;
//			m_options->Parsed.EnvironmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND
//			? XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
//			  XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT
//			: 0;
	layer.viewCount = (uint32_t) projectionLayerViews.size();
	layer.views = projectionLayerViews.data();
	return true;
}

void AppController::renderView(OpenxrView view) {

	glViewport(0, 0, view.getWidth(), view.getHeight());
	AU_CHECK_GL_ERRORS();

//	glFrontFace(GL_CW);
//	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	AU_CHECK_GL_ERRORS();
	glEnable(GL_DEPTH_TEST);
	AU_CHECK_GL_ERRORS();

	scene.render(view);

	glBindVertexArray(0);
	AU_CHECK_GL_ERRORS();
	glUseProgram(0);
	AU_CHECK_GL_ERRORS();
}