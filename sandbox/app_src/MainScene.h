#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include "glm/glm.hpp"
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

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	glm::vec3 cameraPos = glm::vec3(-5.0f, -30.0f, 2.0f);
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

public:
	MainScene(Audace::FileLoader *fileLoader) : fileLoader(fileLoader) {}
	void loadAssets() override;
	void render() override;
	void disposeAssets() override;

	void setVelX(float v) {
		cameraVel.x = v;
	}

	void setVelY(float v) {
		cameraVel.y = v;
	}

	void setVelZ(float v) {
		cameraVel.z = v;
	}
};

#endif