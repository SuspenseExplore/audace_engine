//
// Created by Suspense on 5/5/2023.
//

#include <thread>
#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES3/gl32.h>
#include "EglWindow.h"
#include "renderer/DataBuffer.h"

EglWindow window;
Audace::DataBuffer *buffer;
float verts[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f};

void handleAndroidCmd(android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			window.open(app);
			buffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
			buffer->bind();
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			break;
	}
}

void android_main(android_app *app) {
	app->onAppCmd = handleAndroidCmd;

	while (true) {
		int id;
		int events;
		struct android_poll_source *source;

		while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0) {
			if (source != nullptr) {
				source->process(app, source);
			}
		}

		if (app->destroyRequested != 0) {
			buffer->destroy();
			delete buffer;
			window.close();
			return;
		}

		if (!window.initFinished()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		buffer->bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		window.endFrame();
	}
}