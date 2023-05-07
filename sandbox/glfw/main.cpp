#include <iostream>
#include <string>

#include "glfw3.h"
#include "glad/glad.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"

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

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Audace Sandbox", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);

	glClearColor(1, 0, 1, 0);
	Audace::DataBuffer buffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	buffer.create();
	buffer.bind();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

	Audace::ShaderProgram shader(vsSrc, fsSrc);
	shader.create();
	shader.bind();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glfwSwapBuffers(window);
	}

	shader.destroy();
	buffer.destroy();
	glfwTerminate();
	return 0;
}