#include "au_renderer.h"
#include "GameWindow.h"
#include "Application.h"
#include "AuLogger.h"
#include "spdlog/spdlog.h"

int main() {
	Audace::AuLogger::init();
	AU_ENGINE_LOG_TRACE("Audace logging initialized");
	
	Audace::GameWindow *window = new Audace::GameWindow(1280, 720, "Audace Engine");
	AU_ENGINE_LOG_TRACE("create window");

	Audace::Application *app = Audace::createApp(window);
	AU_ENGINE_LOG_TRACE("application created");

	window->setApplication(app);
	window->open();
	AU_ENGINE_LOG_TRACE("window opened");

	AU_ENGINE_LOG_TRACE("starting app");
	app->start();
	delete app;
	delete window;

	return 0;
}