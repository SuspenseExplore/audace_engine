#include "au_renderer.h"
#include "MainScene.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

void MainScene::loadAssets()
{
	glClearColor(0, 0, 1, 0);
	buffer = new Audace::DataBuffer(verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	buffer->create();
	buffer->bind();

	Audace::VertexAttribute *attr = new Audace::VertexAttribute(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);
	std::vector<Audace::VertexAttribute *> attrs;
	attrs.push_back(attr);

	vertexArray = new Audace::VertexArray(attrs);
	vertexArray->create();
	vertexArray->bind();

	std::string vs = fileLoader->textFileToString("shaders/color/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/color/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();
}

void MainScene::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MainScene::disposeAssets()
{
	buffer->destroy();
	delete buffer;

	vertexArray->destroy();
	delete vertexArray;

	shaderProgram->destroy();
	delete shaderProgram;
}