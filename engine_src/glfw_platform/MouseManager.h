#ifndef AU_MOUSEMANAGER_H
#define AU_MOUSEMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "imgui.h"

namespace Audace
{
	class ButtonInputEvent;
	class BooleanInputEvent;
	class Vec2InputEvent;

	class MouseManager
	{
		static MouseManager *INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;
		std::map<int, std::function<void(BooleanInputEvent)>> buttonChangedEventHandlers;
		std::function<void(Vec2InputEvent)> mouseMoveEventHandler;
		std::function<void(Vec2InputEvent)> mouseWheelEventHandler;

	public:
		static void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler);
		static void addButtonChangedEventHandler(int button, std::function<void(BooleanInputEvent)> handler);
		static void setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> handler);
		static void setMouseWheelEventHandler(std::function<void(Vec2InputEvent)> handler);

		static void setStaticRef(MouseManager *mouseManager) { MouseManager::INSTANCE = mouseManager; };
		static void buttonEventCallback(GLFWwindow *window, int button, int action, int mods);
		static void moveEventCallback(GLFWwindow *window, double x, double y);
		static void wheelEventCallback(GLFWwindow *window, double xOffset, double yOffset);
	};
}

#endif