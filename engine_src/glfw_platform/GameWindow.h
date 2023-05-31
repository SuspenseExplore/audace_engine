#ifndef AU_GAMEWINDOW_H
#define AU_GAMEWINDOW_H

#include "glfw3.h"

namespace Audace {
	class GameWindow {
		GLFWwindow *window;
		int width;
		int height;
		std::string title;

public:
		GameWindow(int width, int height, std::string title) : width(width), height(height), title(title) {}
		bool open();
		bool shouldClose() {return glfwWindowShouldClose(window);}
		void close();

		void beginFrame();
		void endFrame();
	};
}

#endif