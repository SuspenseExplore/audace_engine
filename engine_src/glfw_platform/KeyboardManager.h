#ifndef AU_KEYBOARDMANAGER_H
#define AU_KEYBOARDMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "input/ButtonInputEvent.h"
#include "input/BooleanInputEvent.h"
#include "imgui.h"

namespace Audace
{
	class KeyboardManager
	{
		static KeyboardManager *INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;
		std::map<int, std::function<void(BooleanInputEvent)>> buttonChangedEventHandlers;

	public:
		static void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler) {
			INSTANCE->buttonEventHandlers[button] = handler;
		}
		static void addButtonChangedEventHandler(int button, std::function<void(BooleanInputEvent)> handler) {
			INSTANCE->buttonChangedEventHandlers[button] = handler;
		}

		static void setStaticRef(KeyboardManager *KeyboardManager) { KeyboardManager::INSTANCE = KeyboardManager; };
		static void eventCallback(GLFWwindow *window, int button, int scancode, int action, int mods)
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
	};
}

#endif