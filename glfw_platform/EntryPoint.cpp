#include "au_renderer.h"
#include "GameWindow.h"
#include "Application.h"
#include "AuLogger.h"
#include "spdlog/spdlog.h"

int main() {
	Audace::AuLogger::init();
	AU_ENGINE_LOG_INFO("Audace logging initialized");
	
	Audace::GameWindow *window = new Audace::GameWindow(1280, 720, "Audace Engine");
	Audace::Application *app = Audace::createApp(window);
	window->open();
	app->start();
	delete app;
	delete window;

	return 0;
}