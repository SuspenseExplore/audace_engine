#include "AuLogger.h"
#include "AppController.h"

int main()
{
	Audace::AuLogger::init();
	AU_ENGINE_LOG_TRACE("Logging initialized");

	Audace::AppController appController;

	if (!appController.createWindow(1280, 720, "Audace Sandbox")) {
		return -1;
	}

	appController.runGameLoop();

	appController.shutdown();
	
	AU_ENGINE_LOG_TRACE("Application terminating normally");
	return 0;
}