#include <iostream>

#include "au_platform.h"
#include "GameWindow.h"

namespace Audace
{
	bool GameWindow::open()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		AU_ENGINE_LOG_INFO("GLFW initialized");

		window = glfwCreateWindow(1280, 720, "Audace Sandbox", nullptr, nullptr);
		if (window == nullptr)
		{
			AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
			glfwTerminate();
			return false;
		}
		AU_ENGINE_LOG_INFO("GLFW window created");

		glfwMakeContextCurrent(window);
		AU_ENGINE_LOG_INFO("GL context made current");
		if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
		{
			AU_ENGINE_LOG_CRITICAL("GLAD failed to load");
			glfwTerminate();
			return false;
		}
		AU_ENGINE_LOG_INFO("GLAD loaded");

		glViewport(0, 0, 1280, 720);
		glClearColor(1, 0, 1, 0);

		return true;
	}

	void GameWindow::close()
	{
		glfwTerminate();
	}
}