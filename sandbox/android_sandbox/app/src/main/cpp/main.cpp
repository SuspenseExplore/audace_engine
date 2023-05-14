//
// Created by Suspense on 5/5/2023.
//

#include <thread>
#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES3/gl32.h>
#include <vector>
#include "AuLogger.h"
#include "EglWindow.h"
#include "AppController.h"

Audace::AppController *appController;

void handleAndroidCmd(android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			appController->createWindow();
			AU_RENDERER_LOG_TRACE("Opened window");

			break;
	}
}

void android_main(android_app *app) {
	Audace::AuLogger::init();
	AU_ENGINE_LOG_INFO("Logging initialized");

	appController = new Audace::AppController(app);
	app->onAppCmd = handleAndroidCmd;

	while (!appController->window.initFinished()) {
		Audace::AppController::pollSystemEvents(app);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	AU_ENGINE_LOG_TRACE("Entering render loop");
	appController->runGameLoop();
}