#ifndef AU_GAMEWINDOW_H
#define AU_GAMEWINDOW_H

#include <string>

#include "au_platform.h"
#include "BaseGameWindow.h"

namespace Audace
{
	class GameWindow : public BaseGameWindow
	{
		int32_t width;
		int32_t height;
		std::string title;
		GLFWwindow *window;

	public:
		GameWindow(int32_t width, int32_t height, std::string title) : width(width), height(height), title(title)
		{
			initialized = true;
		}

		~GameWindow() { close(); }
		bool open();
		void close();

		void beginFrame() {}
		void endFrame() { glfwSwapBuffers(window); }
		void processEvents() { glfwPollEvents(); }

		bool shouldClose() { return glfwWindowShouldClose(window); }
	};
}

#endif