#ifndef AU_APPCONTROLLER_H
#define AU_APPCONTROLLER_H

#include <string>
#include "glfw3.h"
#include "EngineEventListener.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

namespace Audace
{
	class AppController : public EngineEventListener
	{

	public:
		FileLoader fileLoader;
		GLFWwindow *window;
		DataBuffer *buffer;
		VertexArray *vertexArray;
		ShaderProgram *shaderProgram;

		AppController() : fileLoader("../../assets/") {}

		bool createWindow(int width, int height, std::string title);
		void pollSystemEvents();
		void runGameLoop();
		void renderFrame();
		void shutdown();

		void windowInitialized() override;
	};
}

#endif