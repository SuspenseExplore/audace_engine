#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"

class MainScene : public Audace::Scene
{
	Audace::FileLoader *fileLoader;
	Audace::DataBuffer *buffer;
	Audace::VertexArray *vertexArray;
	Audace::ShaderProgram *shaderProgram;

public:
	MainScene(Audace::FileLoader *fileLoader) : fileLoader(fileLoader) {}
	void loadAssets() override;
	void render() override;
	void disposeAssets() override;
};

#endif