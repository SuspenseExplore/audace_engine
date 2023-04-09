#include <iostream>

#include "au_renderer.h"
#include "SandboxApp.h"

void SandboxApp::drawFrame() {
	renderContext.render();
}

Audace::Application* Audace::createApp(Audace::BaseGameWindow *window) {
	return new SandboxApp(window);
}