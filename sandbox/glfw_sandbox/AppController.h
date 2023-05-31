#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <string>
#include "GameWindow.h"
#include "EngineEventListener.h"
#include "FileLoader.h"
#include "scene/Scene.h"

namespace Audace
{
	class AppController : public EngineEventListener
	{

	public:
		GameWindow* window;
		FileLoader *fileLoader;
		Scene *scene;

		AppController(){}

		bool createWindow(int width, int height, std::string title);
		void pollSystemEvents();
		void runGameLoop();
		void renderFrame();
		void shutdown();

		void windowInitialized() override;
	};
}

#endif