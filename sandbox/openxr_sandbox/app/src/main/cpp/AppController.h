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
#include "scene/MainScene.h"
#include "SceneEnum.h"

class AppController : public Audace::BaseAppController {
	android_app *androidApp;
	EglWindow window;
	Audace::FileLoader *fileLoader;
	XrFrameState currentFrameState;
	GLuint framebuffer;

	HmdCamera *camera;
	Audace::Scene *scene;
	int nextScene = SandboxScene::CURRENT;

	void startNextScene();

public:
	AppController() : scene(new MainScene(this)) {}
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
	bool renderUiLayer(std::vector<XrCompositionLayerProjectionView> &projectionLayerViews,
					 XrCompositionLayerProjection &layer);

	void renderView(OpenxrView view);
	void renderUiView(OpenxrView view);

	android_app *getAndroidApp() { return androidApp; }

	OpenxrContext getXrContext() { return xrContext; }

	EglWindow getWindow() { return window; }

	void setScene(int nextScene) override;
	int getWidth() override { return 0; }
	int getHeight() override { return 0; }
};


#endif //AUDACE_APPCONTROLLER_H
