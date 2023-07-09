#ifndef AU_DRAGDROPSCENE_H
#define AU_DRAGDROPSCENE_H

#include <vector>
#include "glm/glm.hpp"
#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "renderer/material/SimpleBillboardMaterial.h"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"
#include "scene/ForwardCamera.h"
#include "application/BaseAppController.h"

class DragDropScene : public Audace::Scene
{
	Audace::FileLoader *fileLoader;
	Audace::Sprite *sprite;
	Audace::ShaderProgram *shaderProgram;
	Audace::SimpleBillboardMaterial *material;
	Audace::Texture2d *cardTextures[52];
	std::vector<std::string> suitNames = {"Clubs", "Diamonds", "Hearts", "Spades"};
	std::vector<std::string> rankNames = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

	glm::vec2 mousePos;
	glm::vec2 grabOffset;
	bool dragging = false;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::ForwardCamera camera = Audace::ForwardCamera::standard2d(glm::vec3(0, 0, -1), 1280, 720);

	Audace::RandomUniformFloats rand = Audace::RandomUniformFloats::normalizedRange();

public:
	DragDropScene(Audace::BaseAppController *controller, Audace::FileLoader *fileLoader)
		: Audace::Scene(controller), fileLoader(fileLoader) {}
	Audace::Sprite *loadSprite(std::string filename);
	void loadAssets() override;
	void render() override;
	void disposeAssets() override;

	void setVelX(float v)
	{
		cameraVel.x = v;
	}

	void setVelY(float v)
	{
		cameraVel.y = v;
	}
	void mouseMoved(float x, float y);
	void buttonChanged(bool pressed);

	bool isMouseOverCard(float x, float y);

	Audace::BaseCamera *getCamera() override { return nullptr; }
};

#endif