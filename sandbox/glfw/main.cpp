#include <iostream>

#include "glfw3.h"
#include "glad/glad.h"
#include "renderer/DataBuffer.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

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

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}