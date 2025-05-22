//
// Created by Suspense on 3/4/2023.
//

#include <GLES3/gl32.h>

#include "AppController.h"
#include "content/AssetStore.h"
#include "openxr/openxr_common.h"
#include "glm/glm.hpp"
#include "input/InputDevices.h"
#include "openxr/OpenxrSwapchain.h"
#include "AimIndicator.h"
#include "TeleportTool.h"
#include "renderer/Shapes.h"
#include "imgui.h"

bool AppController::createWindow() {
	fileLoader = new Audace::FileLoader(androidApp->activity->assetManager);
	return window.init(androidApp);
}

bool AppController::init(android_app *app) {
	androidApp = app;
	return xrContext.init(androidApp);
}

bool AppController::createXrSession() {
	Audace::AssetStore::init(fileLoader);
	glGenFramebuffers(1, &framebuffer);
	xrContext.createSession(window.getDisplay(), window.getContext());
	camera = new HmdCamera(xrContext.views);
	scene->setCamera(camera);
	scene->loadAssets(fileLoader);

	using namespace Audace;
//	{
//		OculusTouchController::InputName name = OculusTouchController::InputName::LEFT_GRIP_POSE;
//		xrContext.addPoseInputHandler(name, [this](PoseInputEvent event) {
//			if (event.changed) {
//				((ProcTerrainScene*) scene)->setLightPos(event.state.position);
//			}
//		});
//	}
	{
		OculusTouchController::InputName name = OculusTouchController::InputName::LEFT_AIM_VIEW_POSE;
		xrContext.addPoseInputHandler(name, [this](PoseInputEvent event) {
			if (event.changed) {
				ImGuiIO &io = ImGui::GetIO();
				float x = (event.state.position.x + 1.0) * xrContext.uiSwapchain.getSize().x * 0.5;
				float y = (1.0 - event.state.position.y) * xrContext.uiSwapchain.getSize().y * 0.5;
				io.AddMousePosEvent(x * 1.75, y * 1.75);
			}
		});
	}
	{
		OculusTouchController::InputName name = OculusTouchController::InputName::RIGHT_GRIP_POSE;
		xrContext.addPoseInputHandler(name, [this](PoseInputEvent event) {
			if (event.changed) {
				reinterpret_cast<ProcTerrainScene *>(scene)->setLightPos(event.state.position);
			}
		});
	}
	{
		OculusTouchController::InputName name = OculusTouchController::InputName::LEFT_X_CLICK;
		xrContext.addBooleanInputHandler(name, [this](BooleanInputEvent event) {
			if (event.changed) {
				xButtonDown = event.state;
			}
		});
	}
	xrContext.registerActions();

	SimpleBillboardMaterial *whiteMat = AssetStore::simpleBillboardMaterial();
	SimpleBillboardMaterial *redMat = new SimpleBillboardMaterial();
	redMat->setColor({1.0, 0.0, 0.0, 1.0});
	{
		AimIndicator *leftHandAim = new AimIndicator();
		leftHandAim->getMesh()->setMaterial(whiteMat);
		scene->addSprite(leftHandAim);

		TeleportTool *teleportTool = new TeleportTool();
		teleportTool->getMesh()->setMaterial(whiteMat);
		scene->addSprite(teleportTool);

		addPoseHandler(Audace::OculusTouchController::LEFT_AIM_POSE,
				[=](PoseInputEvent e) {
					leftHandAim->handlePoseEvent(e);
				}
		);
		addPoseHandler(Audace::OculusTouchController::RIGHT_AIM_POSE,
					   [=](PoseInputEvent e) {
						   teleportTool->handlePoseEvent(e);
					   });
		addBooleanEventHandler(Audace::OculusTouchController::RIGHT_A_CLICK,
							 [=](BooleanInputEvent e) {
								 if (teleportTool->isTeleportReady() && e.state) {
									 teleportTool->reset();
									 scene->teleport(teleportTool->getPosition());
								 }
							 });
		addFloatEventHandler(Audace::OculusTouchController::RIGHT_SQUEEZE_VALUE,
							 [=](FloatInputEvent e) {
								 teleportTool->handleSqueezeEvent(e);
							 });
		addFloatEventHandler(Audace::OculusTouchController::RIGHT_THUMBSTICK_Y,
							 [=](FloatInputEvent e) {
								 teleportTool->handleStickMotionEvent(e);
							 });
	}

	return true;
}

void AppController::setScene(int nextScene) {

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

	if (!xrContext.processActions(currentFrameState.predictedDisplayTime)) {
		LOGE("Failed to process XR actions");
		return nullptr;
	}
	ImGuiIO &io = ImGui::GetIO();
	io.AddMouseButtonEvent(0, xButtonDown);
	io.MouseDrawCursor = false;
	return &currentFrameState;
}

bool AppController::endFrame(std::vector<XrCompositionLayerBaseHeader *> layers) {
	XrFrameEndInfo frameEndInfo{XR_TYPE_FRAME_END_INFO};
	frameEndInfo.displayTime = currentFrameState.predictedDisplayTime;
	frameEndInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND;
	frameEndInfo.layerCount = (uint32_t) layers.size();
	frameEndInfo.layers = layers.data();
	XR_ERROR_BAIL("xrEndFrame", xrEndFrame(xrContext.xrSession, &frameEndInfo));

	return true;
}

uint32_t AppController::prepareViews(XrFrameState *frameState, XrSpace space) {
	XrViewState viewState{XR_TYPE_VIEW_STATE};
	auto viewCapacityInput = (uint32_t) xrContext.views.size();
	uint32_t viewCountOutput;

	XrViewLocateInfo viewLocateInfo{XR_TYPE_VIEW_LOCATE_INFO};
	viewLocateInfo.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	viewLocateInfo.displayTime = frameState->predictedDisplayTime;
	viewLocateInfo.space = space;

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
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	window.beginFrame();

	if (frameState != nullptr) {
		std::vector<XrCompositionLayerBaseHeader *> layers;

		std::vector<XrCompositionLayerProjectionView> projectionLayerViews;
		if (frameState->shouldRender == XR_TRUE) {
			uint32_t viewCount = prepareViews(frameState, xrContext.xrWorldSpace);
			if (viewCount > 0) {
				projectionLayerViews.resize(viewCount);
				{
					XrCompositionLayerProjection layer{XR_TYPE_COMPOSITION_LAYER_PROJECTION};
					if (renderLayer(projectionLayerViews, layer)) {
						layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&layer));
					}
				}
			}
			XrCompositionLayerQuad quadLayer{XR_TYPE_COMPOSITION_LAYER_QUAD};
			if (renderUiLayer(quadLayer)) {
				layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&quadLayer));
			}
		}
		window.endFrame();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		endFrame(layers);
	}
}

bool AppController::renderLayer(std::vector<XrCompositionLayerProjectionView> &projectionLayerViews,
								XrCompositionLayerProjection &layer) {
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

		camera->setActiveView(i);
		renderView(view);
		if (!swapchain.endFrame()) {
			return false;
		}
	}

	layer.space = xrContext.xrWorldSpace;
	layer.layerFlags = XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND;
//			m_options->Parsed.EnvironmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND
//			? XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
//			  XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT
//			: 0;
	layer.viewCount = (uint32_t) projectionLayerViews.size();
	layer.views = projectionLayerViews.data();
	return true;
}

bool
AppController::renderUiLayer(XrCompositionLayerQuad &layer) {
	XrResult res;
	OpenxrSwapchain sc = xrContext.uiSwapchain;

	// Each view has a separate swapchain which is acquired, rendered to, and released.
	if (!sc.startFrame()) {
		return false;
	}

	layer.pose.position = {0, 0, -3};
	layer.pose.orientation = {0, 0, 0, 1};
	layer.space = xrContext.xrViewSpace;
	layer.size = {2, 2};
	layer.subImage.swapchain = sc.getHandle();
	layer.subImage.imageRect.offset = {0, 0};
	layer.subImage.imageRect.extent = {sc.getSize().x, sc.getSize().y};
	layer.eyeVisibility = XR_EYE_VISIBILITY_BOTH;
	layer.layerFlags =
			XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
			XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT;

	renderUi();

	if (!sc.endFrame()) {
		return false;
	}
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

	scene->render();

	glBindVertexArray(0);
	AU_CHECK_GL_ERRORS();
	glUseProgram(0);
	AU_CHECK_GL_ERRORS();
}

void AppController::renderUi() {
	glm::ivec2 size = xrContext.uiSwapchain.getSize();
	glViewport(0, 0, size.x, size.y);
	AU_CHECK_GL_ERRORS();

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	AU_CHECK_GL_ERRORS();
	glDisable(GL_DEPTH_TEST);
	AU_CHECK_GL_ERRORS();
	glEnable(GL_BLEND);
	AU_CHECK_GL_ERRORS();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	AU_CHECK_GL_ERRORS();

	glClearColor(0, 0, 0, 0);
	AU_CHECK_GL_ERRORS();
	glClear(GL_COLOR_BUFFER_BIT);
	AU_CHECK_GL_ERRORS();
	ImGuiIO &io = ImGui::GetIO();
	io.MouseDrawCursor = true;
	((ProcTerrainScene *) scene)->renderUi();

	glClearColor(0, 0, 1, 1);
	glBindVertexArray(0);
	AU_CHECK_GL_ERRORS();
	glUseProgram(0);
	AU_CHECK_GL_ERRORS();
}