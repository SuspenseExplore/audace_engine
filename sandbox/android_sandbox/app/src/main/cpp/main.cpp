//
// Created by Suspense on 5/5/2023.
//

#include <thread>
#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES3/gl32.h>
#include "EglWindow.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"

EglWindow window;
Audace::DataBuffer *buffer;
Audace::ShaderProgram *shader;
Audace::VertexAttribute *attr;

float verts[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f};

std::string vsSrc =
		"#version 320 es\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() {\n"
		"	gl_Position = position;\n"
		"}";

std::string fsSrc =
		"#version 320 es\n"
		"precision mediump float;\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"	fragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}";

void handleAndroidCmd(android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			window.open(app);
			buffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
			buffer->create();
			buffer->bind();

			attr = new Audace::VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
			attr->bind();

			shader = new Audace::ShaderProgram(vsSrc, fsSrc);
			shader->create();
			shader->bind();
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
			shader->destroy();
			delete shader;
			delete attr;
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