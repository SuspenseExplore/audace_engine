//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_APPCONTROLLER_H
#define AUDACE_APPCONTROLLER_H

#include <vector>

#include "audace_common.h"
#include "EglWindow.h"
#include "application/BaseAppController.h"
#include "android_platform/FileLoader.h"
#include "openxr/OpenxrContext.h"
#include "scene/Scene.h"
#include "openxr/HmdCamera.h"
#include "input/InputDevices.h"
#include "scene/ProcTerrainScene.h"
#include "scene/SceneBuilder.h"
#include "SceneEnum.h"

class AppController : public Audace::BaseAppController {
	android_app *androidApp;
	EglWindow window;
	Audace::FileLoader *fileLoader;
	XrFrameState currentFrameState;
	GLuint framebuffer;

	HmdCamera *camera;
	Audace::Scene *scene;
	int nextScene = SandboxScene::BUILDER;

	bool xButtonDown = false;

	void startNextScene();

public:
	AppController() : scene(new SceneBuilder(this)) {}
	OpenxrContext xrContext;

	bool init(android_app *app);

	bool createWindow();

	bool createXrSession();

	XrFrameState *startFrame();

	bool endFrame(std::vector<XrCompositionLayerBaseHeader *> layers);

	uint32_t prepareViews(XrFrameState *frameState, XrSpace space);

	void renderFrame();

	bool renderLayer(std::vector<XrCompositionLayerProjectionView> &projectionLayerViews,
					 XrCompositionLayerProjection &layer);
	bool renderUiLayer(XrCompositionLayerQuad &layer);

	void renderView(OpenxrView view);
	void renderUi();

	android_app *getAndroidApp() { return androidApp; }

	OpenxrContext getXrContext() { return xrContext; }

	EglWindow getWindow() { return window; }

	void setScene(int nextScene) override;

	// all the swapchains should have the same dimensions
	int getWidth() override { return xrContext.uiSwapchain.getSize().x; }
	int getHeight() override { return xrContext.uiSwapchain.getSize().y; }


	void addPoseHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::PoseInputEvent)> handler)
	{
		xrContext.addPoseInputHandler(name, handler);
	}

	void addFloatEventHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::FloatInputEvent)> handler)
	{
		xrContext.addFloatInputHandler(name, handler);
	}

	void addBooleanEventHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::BooleanInputEvent)> handler)
	{
		xrContext.addBooleanInputHandler(name, handler);
	}
};


#endif //AUDACE_APPCONTROLLER_H
