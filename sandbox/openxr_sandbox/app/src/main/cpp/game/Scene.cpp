//
// Created by Suspense on 3/5/2023.
//

#include <string>
#include <vector>

#include "audace_common.h"
#include "openxr/xr_linear.h"
#include "Scene.h"

std::string vsSrc = "#version 320 es\n"
					"layout (location = 0) in vec4 position;\n"
					"uniform mat4 mvpMat;"
					"void main() {\n"
					"	gl_Position = mvpMat * position;\n"
					"}";
const char* fsSrc = "#version 320 es\n"
					"precision mediump float;\n"
					"out vec4 color;\n"
					"uniform vec4 uColor;\n"
					"void main() {\n"
					"	color = uColor;\n"
					"}";
void Scene::init() {
	LOGD("SCENE INIT");
	glClearColor(0, 0, 1, 1);
//	glClearDepthf(1.0f);

	vertexBuffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexBuffer->create();
	vertexBuffer->bind();

	attr = new Audace::VertexAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
	std::vector<Audace::VertexAttribute*> attrs;
	attrs.push_back(attr);
	vertexArray = new Audace::VertexArray(attrs);
	vertexArray->create();
	vertexBuffer->unbind();

	shaderProgram = new Audace::ShaderProgram(vsSrc, fsSrc);
	shaderProgram->create();
	shaderProgram->bind();
	mvpMatLocation = glGetUniformLocation(shaderProgram->getId(), "mvpMat");
	colorLocation = glGetUniformLocation(shaderProgram->getId(), "uColor");
}

void Scene::render(OpenxrView view) {
	XrPosef pose = view.getViewData().pose;

	XrMatrix4x4f projMat;
	XrMatrix4x4f_CreateProjectionFov(&projMat, GRAPHICS_OPENGL_ES, view.getViewData().fov, 0.1f, 100.0f);
	XrMatrix4x4f camMat;
	XrVector3f scale{1.f, 1.f, 1.f};
	XrMatrix4x4f_CreateTranslationRotationScale(&camMat, &pose.position, &pose.orientation, &scale);
	XrMatrix4x4f viewMat;
	XrMatrix4x4f_InvertRigidBody(&viewMat, &camMat);
	XrMatrix4x4f vpMat;
	XrMatrix4x4f_Multiply(&vpMat, &projMat, &viewMat);

	shaderProgram->bind();
	glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&vpMat));
	glUniform4f(colorLocation, 1, 0, 0, 0);
	vertexArray->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}