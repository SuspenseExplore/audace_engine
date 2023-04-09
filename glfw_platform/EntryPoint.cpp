#include "au_renderer.h"
#include "GameWindow.h"
#include "Application.h"

int main() {
	Audace::GameWindow *window = new Audace::GameWindow(1280, 720, "Audace Engine");
	Audace::Application *app = Audace::createApp(window);
	window->open();
	app->start();
	delete app;
	delete window;

	return 0;
}