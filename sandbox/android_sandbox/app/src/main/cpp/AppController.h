//
// Created by Suspense on 5/13/2023.
//

#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <jni.h>
#include <android_native_app_glue.h>
#include "EngineEventListener.h"
#include "EglWindow.h"
#include "application/BaseAppController.h"
#include "FileAccessAndroid.h"
#include "scene/MainScene.h"
#include "SceneEnum.h"

namespace Audace
{
	class AppController : public EngineEventListener, BaseAppController
	{
		android_app *androidApp;
		FileAccessAndroid *fileLoader;
		Scene *scene;
		int nextScene = SandboxScene::CURRENT;

		void startNextScene();

	public:
		EglWindow window;

		AppController(android_app *app) : androidApp(app) {}

		bool createWindow();

		void runGameLoop();

		void renderFrame();

		void shutdown();

		void setScene(int newScene) override;

		static void pollSystemEvents(android_app *app);

		void windowInitialized() override;

		int getWidth() override { return window.getWidth(); }
		int getHeight() override { return window.getHeight(); }
        GLuint getDefaultFramebuffer() override { return 0; }
	};
}

#endif // AU_APPCONTROLLER_H
