//
// Created by Suspense on 5/5/2023.
//

#include <thread>
#include <jni.h>
#include <android_native_app_glue.h>
#include <GLES3/gl32.h>
#include <vector>
#include "AuLogger.h"
#include "EglWindow.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

EglWindow window;
Audace::FileLoader *fileLoader;
Audace::DataBuffer *buffer;
Audace::ShaderProgram *shader;
Audace::VertexAttribute *attr;
Audace::VertexArray *vertexArray;

float verts[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f};

void handleAndroidCmd(android_app *app, int32_t cmd) {
	switch (cmd) {
		case APP_CMD_INIT_WINDOW:
			window.open(app);
			AU_RENDERER_LOG_TRACE("Opened window");

			buffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
			buffer->create();
			buffer->bind();

			attr = new Audace::VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
			std::vector<Audace::VertexAttribute*> attrs;
			attrs.push_back(attr);
			vertexArray = new Audace::VertexArray(attrs);
			vertexArray->create();
			buffer->unbind();
			vertexArray->bind();

			std::string vs = fileLoader->textFileToString("shaders/color/vs.glsl");
			std::string fs = fileLoader->textFileToString("shaders/color/fs.glsl");
			shader = new Audace::ShaderProgram(vs, fs);
			shader->create();
			shader->bind();
			break;
	}
}

void android_main(android_app *app) {
	Audace::AuLogger::init();
	AU_ENGINE_LOG_INFO("Logging initialized");
	app->onAppCmd = handleAndroidCmd;
	fileLoader = new Audace::FileLoader(app->activity->assetManager);

	AU_ENGINE_LOG_TRACE("Entering render loop");
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
			vertexArray->destroy();
			delete vertexArray;
			delete fileLoader;

			window.close();
			AU_ENGINE_LOG_TRACE("Exiting render loop");
			AU_ENGINE_LOG_TRACE("Application terminating normally");
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