//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_APPCONTROLLER_H
#define AUDACE_APPCONTROLLER_H

#include <vector>

#include "audace_common.h"
#include "EglWindow.h"
#include "application/BaseAppController.h"
#include "input/InputDevices.h"
#include "openxr/OpenxrContext.h"
#include "SceneEnum.h"

class HmdCamera;
namespace Audace
{
	class FileAccessAndroid;
	class Scene;
}

class AppController : public Audace::BaseAppController
{
	android_app *androidApp;
	EglWindow window;
	Audace::FileAccessAndroid *fileLoader;
	XrFrameState currentFrameState;
	GLuint framebuffer = 0;

	HmdCamera *camera;
	Audace::Scene *scene;
	int nextScene = SandboxScene::GLTF;

	bool xButtonDown = false;

	void startNextScene();

public:
	AppController();
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

	android_app *getAndroidApp();
	OpenxrContext getXrContext();
	EglWindow getWindow();

	void setScene(int nextScene) override;

	// all the swapchains should have the same dimensions
	int getWidth();
	int getHeight();
	GLuint getDefaultFramebuffer() override { return framebuffer; }

	void addPoseHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::PoseInputEvent)> handler);
	void addFloatEventHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::FloatInputEvent)> handler);
	void addBooleanEventHandler(Audace::OculusTouchController::InputName name, std::function<void(Audace::BooleanInputEvent)> handler);
};

#endif // AUDACE_APPCONTROLLER_H
