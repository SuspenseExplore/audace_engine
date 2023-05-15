//
// Created by Suspense on 5/13/2023.
//

#include "AppController.h"

namespace Audace {
	bool AppController::createWindow() {
		window.open(androidApp);
		return true;
	}

	void AppController::windowInitialized() {
		scene = new MainScene(fileLoader);
		scene->loadAssets();
	}

	void AppController::pollSystemEvents(android_app *app) {
		int id;
		int events;
		struct android_poll_source *source;
		while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0) {
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

			renderFrame();
		}
	}

	void AppController::renderFrame() {
		scene->render();
		window.endFrame();
	}

	void AppController::shutdown() {
		scene->disposeAssets();
		delete scene;
	}
}