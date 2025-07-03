#ifndef AU_GAMEWINDOW_H
#define AU_GAMEWINDOW_H

#include <functional>
#include "au_renderer.h"
#include "KeyboardManager.h"
#include "MouseManager.h"
#include "glm/glm.hpp"

class GLFWWindow;

namespace Audace
{
	class ButtonInputEvent;
	class Vec2InputEvent;

	class GameWindow
	{
		GLFWwindow* window;
		int width;
		int height;
		std::string title;

		KeyboardManager keyboardManager;
		MouseManager mouseManager;

	public:
		GameWindow(int width, int height, std::string title);
		bool open();
		bool shouldClose();
		void close();

		void processEvents();
		void beginFrame();
		void endFrame();

		void addKeyEventHandler(int button, std::function<void(ButtonInputEvent)> handler);
		void addMouseButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler);
		void setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> handler);
		bool isMouseButtonDown(int button);

		glm::vec2 getMousePos();
		int getWidth();
		int getHeight();
	};
}

#endif