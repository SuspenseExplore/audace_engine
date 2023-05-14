#include "au_renderer.h"
#include "AppController.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

namespace Audace
{
	bool AppController::createWindow(int width, int height, std::string title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			AU_ENGINE_LOG_CRITICAL("GLFW window creation failed");
			glfwTerminate();
			return false;
		}

		windowInitialized();
		return true;
	}

	void AppController::windowInitialized()
	{
		glfwMakeContextCurrent(window);
		gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
		AU_RENDERER_LOG_TRACE("GLAD loaded");

		glClearColor(1, 0, 1, 0);
		buffer = new DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->create();
		buffer->bind();

		Audace::VertexAttribute *attr = new Audace::VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
		std::vector<Audace::VertexAttribute *> attrs;
		attrs.push_back(attr);

		vertexArray = new Audace::VertexArray(attrs);
		vertexArray->create();
		vertexArray->bind();

		std::string vs = fileLoader.textFileToString("shaders/color/vs.glsl");
		std::string fs = fileLoader.textFileToString("shaders/color/fs.glsl");
		shaderProgram = new Audace::ShaderProgram(vs, fs);
		shaderProgram->create();
		shaderProgram->bind();

		AU_RENDERER_LOG_TRACE("Renderer initialized");
	}

	void AppController::pollSystemEvents()
	{
		glfwPollEvents();
	}

	void AppController::runGameLoop()
	{
		AU_ENGINE_LOG_TRACE("Entering render loop");
		while (true)
		{
			pollSystemEvents();

			if (glfwWindowShouldClose(window)) {
				shutdown();
				return;
			}
			renderFrame();
		}
		AU_ENGINE_LOG_TRACE("Exiting render loop");
	}

	void AppController::renderFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glfwSwapBuffers(window);
	}

	void AppController::shutdown()
	{
		buffer->destroy();
		delete buffer;

		vertexArray->destroy();
		delete vertexArray;

		shaderProgram->destroy();
		delete shaderProgram;

		glfwTerminate();
	}
}