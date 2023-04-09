//
// Created by Suspense on 4/4/2023.
//

#include <android_native_app_glue.h>
#include <GLES3/gl32.h>

#include "EglWindow.h"
#include "Application.h"
#include "au_renderer.h"

Audace::EglWindow *window;
Audace::Application *auApp;

static void handleAndroidCmd(struct android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			window->open();
			glClearColor(1, 0, 1, 0);
			glViewport(0, 0, window->getWidth(), window->getHeight());
			break;

		case APP_CMD_TERM_WINDOW:
			window->close();
			break;
	}
}

void android_main(android_app *app) {
	app->onAppCmd = handleAndroidCmd;
	window = new Audace::EglWindow(app);
	auApp = Audace::createApp(window);
	auApp->start();
	delete auApp;
	delete window;
}