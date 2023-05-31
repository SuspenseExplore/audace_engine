#include "au_renderer.h"
#include "AppController.h"
#include "MainScene.h"

namespace Audace
{
	bool AppController::createWindow(int width, int height, std::string title)
	{
		window = new GameWindow(width, height, title);
		window->open();
		windowInitialized();
		return true;
	}

	void AppController::windowInitialized()
	{
		glEnable(GL_DEPTH_TEST);
		AU_RENDERER_LOG_TRACE("Renderer initialized");

		fileLoader = new FileLoader("../../assets/");

		scene = new MainScene(fileLoader);
		scene->loadAssets();
	}

	void AppController::pollSystemEvents()
	{
		glfwPollEvents();
	}

	void AppController::runGameLoop()
	{
		AU_ENGINE_LOG_TRACE("Entering render loop");
		while (true)
		{
			pollSystemEvents();
			if (window->shouldClose())
			{
				shutdown();
				return;
			}
			window->beginFrame();
			renderFrame();
			window->endFrame();
		}
		AU_ENGINE_LOG_TRACE("Exiting render loop");
	}

	void AppController::renderFrame()
	{
		scene->render();
	}

	void AppController::shutdown()
	{
		delete scene;
		window->close();
	}
}