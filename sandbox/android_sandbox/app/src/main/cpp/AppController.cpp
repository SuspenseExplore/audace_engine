//
// Created by Suspense on 5/13/2023.
//

#include "AppController.h"

float verts[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f};

namespace Audace {
	bool AppController::createWindow() {
		window.open(androidApp);
		buffer = new DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->create();
		buffer->bind();

		VertexAttribute *attr = new VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
		std::vector<VertexAttribute*> attrs;
		attrs.push_back(attr);
		vertexArray = new VertexArray(attrs);
		vertexArray->create();
		buffer->unbind();
		vertexArray->bind();

		std::string vs = fileLoader->textFileToString("shaders/color/vs.glsl");
		std::string fs = fileLoader->textFileToString("shaders/color/fs.glsl");
		shader = new Audace::ShaderProgram(vs, fs);
		shader->create();
		shader->bind();
		return true;
	}

	void AppController::windowInitialized() {

	}

	void AppController::pollSystemEvents(android_app *app) {
		int id;
		int events;
		struct android_poll_source *source;
		while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &source)) >= 0) {
			if (source != nullptr) {
				source->process(app, source);
			}
		}
	}

	void AppController::runGameLoop() {
		while (true) {
			pollSystemEvents(androidApp);

			if (androidApp->destroyRequested != 0) {
				shutdown();
				window.close();
				AU_ENGINE_LOG_TRACE("Exiting render loop");
				AU_ENGINE_LOG_TRACE("Application terminating normally");
				return;
			}

			glClear(GL_COLOR_BUFFER_BIT);
			buffer->bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			window.endFrame();
		}
	}

	void AppController::shutdown() {
		buffer->destroy();
		delete buffer;
		shader->destroy();
		delete shader;
		vertexArray->destroy();
		delete vertexArray;
		delete fileLoader;
	}
}