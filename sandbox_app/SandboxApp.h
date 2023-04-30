#ifndef AU_SANDBOXAPP_H
#define AU_SANDBOXAPP_H

#include "au_platform.h"
#include "Application.h"
#include "BaseGameWindow.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/ShaderProgram.h"

class SandboxApp : public Audace::Application {
	Audace::DataBuffer *buffer;
	Audace::VertexArray *vertexArray;
	Audace::ShaderProgram *shader;

	GLuint bufferId;

public:
	SandboxApp(Audace::BaseGameWindow *window) : Application(window) {}
	~SandboxApp() {}

	void drawFrame();
	void onInit();
	void onShutdown();
};

#endif