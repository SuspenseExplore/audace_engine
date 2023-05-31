//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_APPCONTROLLER_H
#define AUDACE_APPCONTROLLER_H

#include <vector>

#include "audace_common.h"
#include "EglWindow.h"
#include "openxr/OpenxrContext.h"
#include "game/Scene.h"
#include "input/InputDevices.h"
#include "input/BooleanInputHandler.h"

class AppController {
	android_app *androidApp;
	EglWindow window;
	XrFrameState currentFrameState;
	GLuint framebuffer;

	Scene scene;

public:
	OpenxrContext xrContext;

	bool init(android_app *app);

	bool createWindow();

	bool createXrSession();

	XrFrameState *startFrame();

	bool endFrame(std::vector<XrCompositionLayerBaseHeader *> layers);

	uint32_t prepareViews(XrFrameState *frameState);

	void renderFrame();

	bool renderLayer(std::vector<XrCompositionLayerProjectionView> &projectionLayerViews,
					 XrCompositionLayerProjection &layer);

	void renderView(OpenxrView view);

	android_app *getAndroidApp() { return androidApp; }

	OpenxrContext getXrContext() { return xrContext; }

	EglWindow getWindow() { return window; }
};


#endif //AUDACE_APPCONTROLLER_H
