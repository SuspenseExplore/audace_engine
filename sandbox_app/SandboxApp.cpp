#include <string>

#include "au_renderer.h"
#include "SandboxApp.h"

std::string vsSrc = 
"#version 320 es\n"
"layout (location = 0) in vec4 position;\n"
"void main() {\n"
"	gl_Position = position;\n"
"}\0";

std::string fsSrc =
"#version 320 es\n"
"precision mediump float;\n"
"out vec4 fragColor;\n"
"void main() {\n"
"	fragColor = vec4(1.0, 1.0, 0.0, 0.0);\n"
"}\0";

void SandboxApp::onInit() {
	AU_ENGINE_LOG_TRACE("onInit");

	shader = new Audace::ShaderProgram(vsSrc, fsSrc);
	shader->create();
	shader->bind();

	vertexArray = new Audace::VertexArray();
	vertexArray->create();
	vertexArray->bind();

	float verts[] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5, 0.5
	};
	buffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	buffer->create();
	buffer->bind();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

	vertexArray->unbind();
	buffer->unbind();
}

void SandboxApp::drawFrame() {
	renderContext.render();
	vertexArray->bind();
	CALL_GL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
}

void SandboxApp::onShutdown() {
	delete buffer;
	delete vertexArray;
}

Audace::Application* Audace::createApp(Audace::BaseGameWindow *window) {
	return new SandboxApp(window);
}
