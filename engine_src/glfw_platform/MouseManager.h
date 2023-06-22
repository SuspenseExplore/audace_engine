#ifndef AU_MOUSEMANAGER_H
#define AU_MOUSEMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "input/ButtonInputEvent.h"
#include "input/BooleanInputEvent.h"
#include "input/Vec2InputEvent.h"
#include "imgui.h"

namespace Audace
{
	class MouseManager
	{
		static MouseManager *INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;
		std::map<int, std::function<void(BooleanInputEvent)>> buttonChangedEventHandlers;
		std::function<void(Vec2InputEvent)> mouseMoveEventHandler;

	public:
		static void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
		{
			INSTANCE->buttonEventHandlers[button] = handler;
		}
		static void addButtonChangedEventHandler(int button, std::function<void(BooleanInputEvent)> handler)
		{
			INSTANCE->buttonChangedEventHandlers[button] = handler;
		}
		static void setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> *handler)
		{
			INSTANCE->mouseMoveEventHandler = *handler;
		}

		static void setStaticRef(MouseManager *mouseManager) { MouseManager::INSTANCE = mouseManager; };
		static void buttonEventCallback(GLFWwindow *window, int button, int action, int mods)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureMouse) return;
			
			if (INSTANCE->buttonEventHandlers.find(button) != INSTANCE->buttonEventHandlers.end())
			{
				ButtonInputEvent event(button, action == GLFW_PRESS, true, std::chrono::high_resolution_clock::now().time_since_epoch().count());
				INSTANCE->buttonEventHandlers[button](event);
			}
			if (INSTANCE->buttonChangedEventHandlers.find(button) != INSTANCE->buttonChangedEventHandlers.end())
			{
				BooleanInputEvent event(action == GLFW_PRESS, true, std::chrono::high_resolution_clock::now().time_since_epoch().count());
				INSTANCE->buttonChangedEventHandlers[button](event);
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