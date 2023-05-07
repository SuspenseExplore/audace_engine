//
// Created by Suspense on 3/4/2023.
//

#include <memory>
#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "EglWindow.h"

bool EglWindow::open(android_app* app) {
	const EGLint attrs[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
							EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
							EGL_BLUE_SIZE, 8,
							EGL_GREEN_SIZE, 8,
							EGL_RED_SIZE, 8,
							EGL_DEPTH_SIZE, 24,
							EGL_NONE};
	EGLint format;
	EGLint numCfg;
	EGLConfig config = nullptr;

	EGLint major, minor;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, &major, &minor);
	eglChooseConfig(display, attrs, nullptr, 0, &numCfg);
	std::unique_ptr<EGLConfig[]> supportedCfgs(new EGLConfig[numCfg]);
	eglChooseConfig(display, attrs, supportedCfgs.get(), numCfg, &numCfg);

	auto i = 0;
	for (; i < numCfg; i++) {
		auto &cfg = supportedCfgs[i];
		EGLint r, g, b, d;
		eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r);
		eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g);
		eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b);
		eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d);
		if (r == 8 && g == 8 && b == 8 && d == 24) {
			config = supportedCfgs[i];
			break;
		}
	}
	if (i == numCfg) {
		config = supportedCfgs[0];
	}
	if (config == nullptr) {
		return false;
	}
	const EGLint contextAttrs[] = {EGL_CONTEXT_MAJOR_VERSION, 3,
								   EGL_CONTEXT_MINOR_VERSION, 2,
								   EGL_NONE};

	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	surface = eglCreateWindowSurface(display, config, app->window, nullptr);
	context = eglCreateContext(display, config, nullptr, contextAttrs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		return false;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &height);

	auto glInfo = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
	for (auto name: glInfo) {
		auto info = glGetString(name);
	}

	glClearColor(1, 0, 1, 0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	initialized = true;

	return true;
}

void EglWindow::close() {

	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	}
	if (context != EGL_NO_CONTEXT) {
		eglDestroyContext(display, context);
	}
	if (surface != EGL_NO_SURFACE) {
		eglDestroySurface(display, surface);
	}
	eglTerminate(display);
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}