#ifndef AU_KEYBOARDMANAGER_H
#define AU_KEYBOARDMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "imgui.h"

namespace Audace
{
	class ButtonInputEvent;
	class BooleanInputEvent;

	class KeyboardManager
	{
		static KeyboardManager* INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;
		std::map<int, std::function<void(BooleanInputEvent)>> buttonChangedEventHandlers;

	public:
		static void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler) {
			INSTANCE->buttonEventHandlers[button] = handler;
		}
		static void addButtonChangedEventHandler(int button, std::function<void(BooleanInputEvent)> handler) {
			INSTANCE->buttonChangedEventHandlers[button] = handler;
		}

		static void setStaticRef(KeyboardManager* KeyboardManager) { KeyboardManager::INSTANCE = KeyboardManager; };
		static void eventCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
	};
}

#endif