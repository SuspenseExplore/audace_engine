#ifndef AU_APPLICATION_H
#define AU_APPLICATION_H

#include "au_platform.h"
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
			
			onShutdown();
			window->close();
		}

		virtual void stop() {}

		virtual void onInit() {}
		virtual void drawFrame() {}
		virtual void onShutdown() {}
	};
}

#endif