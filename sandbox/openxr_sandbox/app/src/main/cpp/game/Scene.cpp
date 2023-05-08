//
// Created by Suspense on 3/5/2023.
//

#include <string>

#include "audace_common.h"
#include "openxr/xr_linear.h"
#include "Scene.h"

void Scene::init() {
	LOGD("SCENE INIT");
	glClearColor(0, 0, 1, 1);
//	glClearDepthf(1.0f);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsSrc = "#version 320 es\n"
						"layout (location = 0) in vec4 position;\n"
						"uniform mat4 mvpMat;"
						"void main() {\n"
						"	gl_Position = mvpMat * position;\n"
						"}\0";
	glShaderSource(vertexShader, 1, &vsSrc, nullptr);
	glCompileShader(vertexShader);
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, log);
		LOGE("Vertex shader compilation error: %s", log);
	}

	const char* fsSrc = "#version 320 es\n"
						"precision mediump float;\n"
						"out vec4 color;\n"
						"uniform vec4 uColor;\n"
						"void main() {\n"
						"	color = uColor;\n"
						"}\0";
	glShaderSource(fragmentShader, 1, &fsSrc, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
		LOGE("Fragment shader compilation error: %s", log);
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
		LOGE("Shader link error: %s", log);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	mvpMatLocation = glGetUniformLocation(shaderProgram, "mvpMat");
	colorLocation = glGetUniformLocation(shaderProgram, "uColor");
	LOGD("uniform location: %d", mvpMatLocation);
	LOGD("uniform location: %d", colorLocation);
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

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&vpMat));
	glUniform4f(colorLocation, 1, 0, 0, 0);
	glBindVertexArray(vertexArray);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}