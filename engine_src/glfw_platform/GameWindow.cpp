#include <string>
#include "au_renderer.h"
#include "AuLogger.h"
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

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
		AU_RENDERER_LOG_TRACE("GLAD loaded");
		return true;
	}

	void GameWindow::close()
	{
		glfwTerminate();
	}

	void GameWindow::beginFrame() {

	}

	void GameWindow::endFrame() {
		glfwSwapBuffers(window);
	}
}