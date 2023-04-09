//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_EGLWINDOW_H
#define AUDACE_EGLWINDOW_H

#include <EGL/egl.h>
#include <android_native_app_glue.h>

#include "BaseGameWindow.h"

namespace Audace {
	class EglWindow : public BaseGameWindow {
		android_app *androidApp;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;

		int32_t width;
		int32_t height;

	public:
		EglWindow(android_app *app) : BaseGameWindow() {androidApp = app;}
		bool open();

		void close();

		void beginFrame();

		void endFrame();

		EGLDisplay getDisplay() { return display; }

		EGLSurface getSurface() { return surface; }

		EGLContext getContext() { return context; }

		int32_t getWidth() { return width; }

		int32_t getHeight() { return height; }

		void processEvents();

		bool shouldClose() { return androidApp->destroyRequested; }
	};
}


#endif //AUDACE_EGLWINDOW_H
