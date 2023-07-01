#ifndef AU_GAMEWINDOW_H
#define AU_GAMEWINDOW_H

#include <functional>
#include "glfw3.h"
#include "KeyboardManager.h"
#include "MouseManager.h"
#include "input/ButtonInputEvent.h"
#include "input/Vec2InputEvent.h"

namespace Audace
{
	class GameWindow
	{
		GLFWwindow *window;
		int width;
		int height;
		std::string title;

		KeyboardManager keyboardManager;
		MouseManager mouseManager;

	public:
		GameWindow(int width, int height, std::string title);
		bool open();
		bool shouldClose() { return glfwWindowShouldClose(window); }
		void close();

		void processEvents();
		void beginFrame();
		void endFrame();

		void addKeyEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
		{
			keyboardManager.addButtonEventHandler(button, handler);
		}

		void addMouseButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
		{
			mouseManager.addButtonEventHandler(button, handler);
		}

		void setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> handler)
		{
			mouseManager.setMouseMoveEventHandler(&handler);
		}

		bool isMouseButtonDown(int button) { return glfwGetMouseButton(window, button) == GLFW_PRESS; };

		glm::vec2 getMousePos() {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			return glm::vec2(x, y);
		}

		int getWidth() { return width; }
		int getHeight() { return height; }
	};
}

#endif