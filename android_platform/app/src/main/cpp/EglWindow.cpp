//
// Created by Suspense on 3/4/2023.
//

#include <memory>
#include <GLES/gl.h>
#include <GLES3/gl32.h>

#include "EglWindow.h"
#include "AuLogger.h"

namespace Audace {
	bool EglWindow::open() {
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
		AU_ENGINE_LOG_INFO("EGL version: {0:d}.{1:d}", major, minor);

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
			AU_ENGINE_LOG_CRITICAL("EGL could not find valid config");
			return false;
		}
		const EGLint contextAttrs[] = {EGL_CONTEXT_MAJOR_VERSION, 3,
									   EGL_CONTEXT_MINOR_VERSION, 2,
									   EGL_NONE};

		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
		surface = eglCreateWindowSurface(display, config, androidApp->window, nullptr);
		context = eglCreateContext(display, config, nullptr, contextAttrs);

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
			AU_ENGINE_LOG_CRITICAL("eglMakeCurrent failed during init");
			return false;
		}

		eglQuerySurface(display, surface, EGL_WIDTH, &width);
		eglQuerySurface(display, surface, EGL_HEIGHT, &height);

		auto glInfo = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
		for (auto name: glInfo) {
			const char *info = reinterpret_cast<const char *>(glGetString(name));
			AU_ENGINE_LOG_INFO("GL info: {}", info);
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		glEnable(GL_CULL_FACE);
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

	void EglWindow::beginFrame() {
		AU_ENGINE_LOG_TRACE("Starting new frame");
	}

	void EglWindow::endFrame() {
		AU_ENGINE_LOG_TRACE("Ending frame");
		eglSwapBuffers(display, surface);
	}

	void EglWindow::processEvents() {
		int id;
		int events;
		struct android_poll_source *source;

		while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0) {
			if (source != nullptr) {
				source->process(androidApp, source);
			}
		}
	}
}