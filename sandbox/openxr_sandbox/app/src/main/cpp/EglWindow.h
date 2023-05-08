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

public:
	bool init(android_app* app);
	void close();

	EGLDisplay getDisplay() {return display;}
	EGLSurface getSurface() {return surface;}
	EGLContext getContext() {return context;}
	int32_t getWidth() {return width;}
	int32_t getHeight() {return height;}
};


#endif //AUDACE_EGLWINDOW_H
