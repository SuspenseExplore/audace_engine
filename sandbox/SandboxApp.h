#ifndef AU_SANDBOXAPP_H
#define AU_SANDBOXAPP_H

#include "Application.h"
#include "BaseGameWindow.h"

class SandboxApp : public Audace::Application {

public:
	SandboxApp(Audace::BaseGameWindow *window) : Application(window) {}

	void drawFrame();
};

#endif