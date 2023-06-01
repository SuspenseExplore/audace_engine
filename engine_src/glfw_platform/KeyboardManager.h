#ifndef AU_KEYBOARDMANAGER_H
#define AU_KEYBOARDMANAGER_H

#include <map>
#include <functional>
#include "glfw3.h"
#include "AuLogger.h"
#include "input/KeyInputEvent.h"

namespace Audace
{
	class KeyboardManager
	{
		static KeyboardManager *INSTANCE;
		std::map<int, std::function<void(KeyInputEvent)>> keyEventHandlers;

	public:
		void addKeyEventHandler(int key, std::function<void(KeyInputEvent)> handler) {
			keyEventHandlers[key] = handler;
		}

		static void setStaticRef(KeyboardManager *KeyboardManager) { KeyboardManager::INSTANCE = KeyboardManager; };
		static void eventCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			if (INSTANCE->keyEventHandlers.find(key) != INSTANCE->keyEventHandlers.end())
			{
				KeyInputEvent event(key, action != GLFW_RELEASE, action != GLFW_REPEAT, std::chrono::high_resolution_clock::now().time_since_epoch().count());
				INSTANCE->keyEventHandlers[key](event);
			}
		}
	};
}

#endif