#include <chrono>
#include "AuLogger.h"
#include "glfw_platform/KeyboardManager.h"
#include "input/ButtonInputEvent.h"
#include "input/BooleanInputEvent.h"

Audace::KeyboardManager* Audace::KeyboardManager::INSTANCE;

namespace Audace {
	void KeyboardManager::eventCallback(GLFWwindow *window, int button, int scancode, int action, int mods)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard) return;

		if (INSTANCE->buttonEventHandlers.find(button) != INSTANCE->buttonEventHandlers.end())
		{
			ButtonInputEvent event(button, action != GLFW_RELEASE, action != GLFW_REPEAT, std::chrono::high_resolution_clock::now().time_since_epoch().count());
			INSTANCE->buttonEventHandlers[button](event);
		}

		if (action != GLFW_REPEAT && INSTANCE->buttonChangedEventHandlers.find(button) != INSTANCE->buttonChangedEventHandlers.end()) {
			INSTANCE->buttonChangedEventHandlers[button](BooleanInputEvent(action == GLFW_PRESS, true, std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
	}
}