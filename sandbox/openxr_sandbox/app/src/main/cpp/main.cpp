#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <map>
#include <thread>
#include <chrono>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>

#include "AuLogger.h"
#include "AppController.h"
#include "EglWindow.h"
#include "openxr/openxr_common.h"
#include "openxr/OpenxrContext.h"
#include "openxr/OpenxrView.h"
#include "openxr/OpenxrSwapchain.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_android.h"

AppController appController;
GLuint framebuffer;

bool sessionRunning = false;
bool exitRenderLoop = false;

static int initWindow() {
	appController.createWindow();
	appController.createXrSession();
	glGenFramebuffers(1, &framebuffer);

	return 0;
}

static void handleAndroidCmd(struct android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
//			core->app->savedState = malloc(sizeof(struct app_state));
//			*((struct app_state *) core->app->savedState) = core->appState;
//			core->app->savedStateSize = sizeof(struct app_state);
//			LOGD("saved state");
			break;

		case APP_CMD_INIT_WINDOW:
			LOGD("createViews window");
			if (app->window != nullptr) {
				initWindow();
			}
			break;

		case APP_CMD_TERM_WINDOW:
			LOGD("term window");
			appController.getWindow().close();
			glDeleteFramebuffers(1, &framebuffer);
			break;
	}
}

int32_t handleInputEvent(android_app* app, AInputEvent* event) {
	return ImGui_ImplAndroid_HandleInputEvent(event);
}

static void handleXrEvents() {
	XrEventDataBuffer eventBuffer;
	auto *event = reinterpret_cast<XrEventDataBaseHeader *>(&eventBuffer);
	*event = {XR_TYPE_EVENT_DATA_BUFFER};
	XrResult res = XR_SUCCESS;

	while ((res = xrPollEvent(appController.xrContext.instance, &eventBuffer)) != XR_EVENT_UNAVAILABLE) {
		switch (event->type) {
			case XR_TYPE_EVENT_DATA_EVENTS_LOST: {
				auto *eventsLost = reinterpret_cast<const XrEventDataEventsLost *>(event);
				LOGD("XR events lost: %d", eventsLost->lostEventCount);
				break;
			}

			case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
				const auto &instanceLossPending = *reinterpret_cast<const XrEventDataInstanceLossPending *>(event);
				exitRenderLoop = true;
				LOGD("XR instance loss pending");
				break;
			}

			case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
				auto sessionStateChangedEvent = *reinterpret_cast<const XrEventDataSessionStateChanged *>(event);
				switch (sessionStateChangedEvent.state) {
					case XR_SESSION_STATE_READY: {
						if (appController.xrContext.beginSession()) {
							sessionRunning = true;
						}
						break;
					}
					case XR_SESSION_STATE_STOPPING: {
						if (appController.xrContext.xrSession != XR_NULL_HANDLE) {
							sessionRunning = false;
							res = xrEndSession(appController.xrContext.xrSession);
							if (res != XR_SUCCESS) {
								LOGE("xrEndSession failed: %d", res);
								break;
							}
							sessionRunning = false;
						}
						break;
					}
					case XR_SESSION_STATE_FOCUSED: {
						break;
					}
					case XR_SESSION_STATE_EXITING: {
						exitRenderLoop = true;
						// Do not attempt to restart because user closed this session.
						break;
					}
					case XR_SESSION_STATE_LOSS_PENDING: {
						exitRenderLoop = true;
						// Poll for a new instance.
						break;
					}
					default:
						break;
				}
				break;
			}

			default: {
				LOGD("Ignoring event type %d", event->type);
				break;
			}
		}
		*event = {XR_TYPE_EVENT_DATA_BUFFER};
	}
}

void android_main(struct android_app *app) {
	Audace::AuLogger::init();
	AU_ENGINE_LOG_INFO("Logging initialized");

	app->onAppCmd = handleAndroidCmd;
	app->onInputEvent = handleInputEvent;
	appController.init(app);

	AU_RENDERER_LOG_TRACE("Entering render loop");
	while (true) {
		int id;
		int events;
		struct android_poll_source *source;

		while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0) {
			if (source != nullptr) {
				source->process(app, source);
			}
		}

		handleXrEvents();
		if (exitRenderLoop) {
			appController.getWindow().close();
			glDeleteFramebuffers(1, &framebuffer);
			ANativeActivity_finish(app->activity);

			AU_ENGINE_LOG_TRACE("Application terminating normally");
			return;
		}

		if (app->destroyRequested != 0) {
			appController.getWindow().close();
			glDeleteFramebuffers(1, &framebuffer);

			AU_ENGINE_LOG_TRACE("Application terminating normally");
			return;
		}

		if (!sessionRunning) {
			// Throttle loop since xrWaitFrame won't be called.
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			continue;
		}

		appController.renderFrame();
	}
}