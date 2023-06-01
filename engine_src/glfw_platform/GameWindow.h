#ifndef AU_GAMEWINDOW_H
#define AU_GAMEWINDOW_H

#include <functional>
#include "glfw3.h"
#include "KeyboardManager.h"
#include "input/KeyInputEvent.h"

namespace Audace {
	class GameWindow {
		GLFWwindow *window;
		int width;
		int height;
		std::string title;

		KeyboardManager keyboardManager;

public:
		GameWindow(int width, int height, std::string title);
		bool open();
		bool shouldClose() {return glfwWindowShouldClose(window);}
		void close();

		void processEvents();
		void beginFrame();
		void endFrame();

		void addKeyEventHandler(int key, std::function<void(KeyInputEvent)> handler) {
			keyboardManager.addKeyEventHandler(key, handler);
		}
	};
}

#endif