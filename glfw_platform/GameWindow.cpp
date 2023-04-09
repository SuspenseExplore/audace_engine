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

		window = glfwCreateWindow(1280, 720, "Audace Sandbox", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cerr << "glfwCreateWindow failed" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "GLAD failed to load" << std::endl;
			glfwTerminate();
			return false;
		}

		glViewport(0, 0, 1280, 720);
		glClearColor(1, 0, 1, 0);

		return true;
	}

	void GameWindow::close()
	{
		glfwTerminate();
	}
}