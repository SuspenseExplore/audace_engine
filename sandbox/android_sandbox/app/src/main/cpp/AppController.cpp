//
// Created by Suspense on 5/13/2023.
//

#include "AuLogger.h"
#include "AppController.h"
#include "scene/NavigationScene.h"
#include "scene/SceneBuilder.h"

namespace Audace {
	bool AppController::createWindow() {
		window.open(androidApp);
		return true;
	}

	void AppController::windowInitialized() {
		scene = new NavigationScene(this);
		scene->loadAssets(fileLoader);
	}

	void AppController::pollSystemEvents(android_app *app) {
		int id;
		int events;
		struct android_poll_source *source;
		while ((id = ALooper_pollOnce(0, nullptr, &events, (void **) &source)) >= 0) {
			if (source != nullptr) {
				source->process(app, source);
			}
		}
	}

	void AppController::runGameLoop() {
		while (true) {
			pollSystemEvents(androidApp);

			if (androidApp->destroyRequested != 0) {
				shutdown();
				window.close();
				AU_ENGINE_LOG_TRACE("Exiting render loop");
				AU_ENGINE_LOG_TRACE("Application terminating normally");
				return;
			}
			if (nextScene != CURRENT) {
				startNextScene();
			}

			renderFrame();
		}
	}

	void AppController::setScene(int scene)
	{
		nextScene = scene;
	}

	void AppController::startNextScene()
	{
		scene->disposeAssets();
		delete scene;

		switch (nextScene)
		{
			case NAVIGATION:
				scene = new NavigationScene(this);
				scene->loadAssets(fileLoader);
				break;

			case MAIN:
				scene = new MainScene(this);
				scene->loadAssets(fileLoader);
				break;

			case BUILDER:
				scene = new SceneBuilder(this);
				scene->loadAssets(fileLoader);
				break;
		}
		nextScene = CURRENT;
	}

	void AppController::renderFrame() {
		window.beginFrame();
		scene->render();
		window.endFrame();
	}

	void AppController::shutdown() {
		scene->disposeAssets();
		delete scene;
	}
}