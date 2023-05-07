//
// Created by Suspense on 3/4/2023.
//

#ifndef AUDACE_EGLWINDOW_H
#define AUDACE_EGLWINDOW_H

#include <EGL/egl.h>
#include <android_native_app_glue.h>

class EglWindow {
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;

	int32_t width;
	int32_t height;

	bool initialized = false;

public:
	bool open(android_app* app);
	void close();

	void endFrame() { eglSwapBuffers(display, surface);}

	EGLDisplay getDisplay() {return display;}
	EGLSurface getSurface() {return surface;}
	EGLContext getContext() {return context;}
	int32_t getWidth() {return width;}
	int32_t getHeight() {return height;}
	bool initFinished() {return initialized;}
};


#endif //AUDACE_EGLWINDOW_H
