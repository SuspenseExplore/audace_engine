#ifndef AU_APPLICATION_H
#define AU_APPLICATION_H

#include "BaseGameWindow.h"
#include "RenderContext.h"

namespace Audace
{
	class Application
	{
	protected:
		BaseGameWindow *window;
		RenderContext renderContext;

	public:
		Application(BaseGameWindow *window) : window(window) {}
		virtual ~Application() {}

		virtual void start()
		{
			while (!window->shouldClose())
			{
				window->processEvents();

				if (window->isInitialized())
				{
					window->beginFrame();
					drawFrame();
					window->endFrame();
				}
			}

			window->close();
		}

		virtual void stop() {}

		virtual void drawFrame() {}
	};
}

#endif