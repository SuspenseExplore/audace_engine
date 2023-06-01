#ifndef AU_MOUSEMANAGER_H
#define AU_MOUSEMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "input/ButtonInputEvent.h"
#include "input/Vec2InputEvent.h"

namespace Audace
{
	class MouseManager
	{
		static MouseManager *INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;
		std::function<void(Vec2InputEvent)> mouseMoveEventHandler;

	public:
		void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
		{
			buttonEventHandlers[button] = handler;
		}
		void setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> *handler)
		{
			mouseMoveEventHandler = *handler;
		}

		static void setStaticRef(MouseManager *mouseManager) { MouseManager::INSTANCE = mouseManager; };
		static void buttonEventCallback(GLFWwindow *window, int button, int action, int mods)
		{
			if (INSTANCE->buttonEventHandlers.find(button) != INSTANCE->buttonEventHandlers.end())
			{
				ButtonInputEvent event(button, action != GLFW_RELEASE, true, std::chrono::high_resolution_clock::now().time_since_epoch().count());
				INSTANCE->buttonEventHandlers[button](event);
			}
		}
		static void moveEventCallback(GLFWwindow *window, double x, double y)
		{
			if (INSTANCE->mouseMoveEventHandler != nullptr)
			{
				INSTANCE->mouseMoveEventHandler(Vec2InputEvent(glm::vec2(x, y), true, std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			}
		}
	};
}

#endif