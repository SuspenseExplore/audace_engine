#include <iostream>
#include <string>
#include <vector>

#include "AuLogger.h"
#include "glfw3.h"
#include "glad/glad.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

int main()
{
	Audace::AuLogger::init();
	AU_ENGINE_LOG_TRACE("Logging initialized");

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Audace Sandbox", nullptr, nullptr);
	if (window == nullptr)
	{
		AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
	AU_RENDERER_LOG_TRACE("GLAD loaded");

	glClearColor(1, 0, 1, 0);
	Audace::DataBuffer buffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	buffer.create();
	buffer.bind();

	Audace::VertexAttribute *attr = new Audace::VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
	std::vector<Audace::VertexAttribute *> attrs;
	attrs.push_back(attr);

	Audace::VertexArray vertexArray(attrs);
	vertexArray.create();
	buffer.unbind();
	vertexArray.bind();

	Audace::FileLoader fileLoader("../../assets/");
	std::string vs = fileLoader.textFileToString("shaders/color/vs.glsl");
	std::string fs = fileLoader.textFileToString("shaders/color/fs.glsl");
	Audace::ShaderProgram shader(vs, fs);
	shader.create();
	shader.bind();

	AU_ENGINE_LOG_TRACE("Entering render loop");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glfwSwapBuffers(window);
	}
	AU_ENGINE_LOG_TRACE("Exiting render loop");

	shader.destroy();
	buffer.destroy();
	vertexArray.destroy();
	glfwTerminate();
	
	AU_ENGINE_LOG_TRACE("Application terminating normally");
	return 0;
}