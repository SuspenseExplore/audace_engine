#include "au_renderer.h"
#include "AppController.h"
#include "MainScene.h"

namespace Audace
{
	bool AppController::createWindow(int width, int height, std::string title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
			glfwTerminate();
			return false;
		}

		windowInitialized();
		return true;
	}

	void AppController::windowInitialized()
	{
		glfwMakeContextCurrent(window);
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
		AU_RENDERER_LOG_TRACE("GLAD loaded");

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

			if (glfwWindowShouldClose(window))
			{
				shutdown();
				return;
			}
			renderFrame();
		}
		AU_ENGINE_LOG_TRACE("Exiting render loop");
	}

	void AppController::renderFrame()
	{
		scene->render();
		glfwSwapBuffers(window);
	}

	void AppController::shutdown()
	{
		delete scene;
		glfwTerminate();
	}
}