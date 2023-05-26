#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"

class MainScene : public Audace::Scene
{
	Audace::FileLoader *fileLoader;
	Audace::Sprite *boxSprite;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *darkGridTex;
	Audace::Texture2d *greenChecksTex;
	Audace::Texture2d *orangeChecksTex;
	Audace::Texture2d *purpleChecksTex;

public:
	MainScene(Audace::FileLoader *fileLoader) : fileLoader(fileLoader) {}
	void loadAssets() override;
	void render() override;
	void disposeAssets() override;
};

#endif