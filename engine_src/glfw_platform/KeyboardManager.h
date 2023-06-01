#ifndef AU_KEYBOARDMANAGER_H
#define AU_KEYBOARDMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "input/ButtonInputEvent.h"

namespace Audace
{
	class KeyboardManager
	{
		static KeyboardManager *INSTANCE;
		std::map<int, std::function<void(ButtonInputEvent)>> buttonEventHandlers;

	public:
		void addButtonEventHandler(int button, std::function<void(ButtonInputEvent)> handler) {
			buttonEventHandlers[button] = handler;
		}

		static void setStaticRef(KeyboardManager *KeyboardManager) { KeyboardManager::INSTANCE = KeyboardManager; };
		static void eventCallback(GLFWwindow *window, int button, int scancode, int action, int mods)
		{
			if (INSTANCE->buttonEventHandlers.find(button) != INSTANCE->buttonEventHandlers.end())
			{
				ButtonInputEvent event(button, action != GLFW_RELEASE, action != GLFW_REPEAT, std::chrono::high_resolution_clock::now().time_since_epoch().count());
				INSTANCE->buttonEventHandlers[button](event);
			}
		}
	};
}

#endif