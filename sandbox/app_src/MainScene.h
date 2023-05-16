#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Texture2d.h"

class MainScene : public Audace::Scene
{
	Audace::FileLoader *fileLoader;
	Audace::DataBuffer *buffer;
	Audace::VertexArray *vertexArray;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *texture;

public:
	MainScene(Audace::FileLoader *fileLoader) : fileLoader(fileLoader) {}
	~MainScene() override;
	void loadAssets() override;
	void render() override;
	void disposeAssets() override;
};

#endif