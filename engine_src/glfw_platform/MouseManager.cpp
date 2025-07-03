#include <chrono>
#include "AuLogger.h"
#include "glfw_platform/MouseManager.h"
#include "input/ButtonInputEvent.h"
#include "input/BooleanInputEvent.h"
#include "input/Vec2InputEvent.h"
#include "glm/glm.hpp"

Audace::MouseManager* Audace::MouseManager::INSTANCE;

namespace Audace
{
	void MouseManager::addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler)
	{
		INSTANCE->buttonEventHandlers[button] = handler;
	}

	void MouseManager::addButtonChangedEventHandler(int button, std::function<void(BooleanInputEvent)> handler)
	{
		INSTANCE->buttonChangedEventHandlers[button] = handler;
	}

	void MouseManager::setMouseMoveEventHandler(std::function<void(Vec2InputEvent)> handler)
	{
		INSTANCE->mouseMoveEventHandler = handler;
	}

	void MouseManager::setMouseWheelEventHandler(std::function<void(Vec2InputEvent)> handler)
	{
		INSTANCE->mouseWheelEventHandler = handler;
	}

	void MouseManager::buttonEventCallback(GLFWwindow* window, int button, int action, int mods)
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
	void MouseManager::moveEventCallback(GLFWwindow* window, double x, double y)
	{
		if (INSTANCE->mouseMoveEventHandler != nullptr)
		{
			INSTANCE->mouseMoveEventHandler(Vec2InputEvent(glm::vec2(x, y), true, std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
	}
	void MouseManager::wheelEventCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		if (INSTANCE->mouseWheelEventHandler != nullptr)
		{
			INSTANCE->mouseWheelEventHandler(Vec2InputEvent(glm::vec2(xOffset, yOffset), true, std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
	}
}