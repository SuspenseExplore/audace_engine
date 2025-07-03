#ifndef AU_DRAGDROPSCENE_H
#define AU_DRAGDROPSCENE_H

#include <vector>
#include "glm/glm.hpp"
#include "scene/Scene.h"
#include "util/RandomUniformFloats.h"

namespace Audace
{
	class ShaderProgram;
	class SimpleBillboardMaterial;
	class Texture2d;
	class ForwardCamera;
}

class DragDropScene : public Audace::Scene
{
	Audace::IFileAccess* fileLoader;
	Audace::Sprite* sprite;
	Audace::ShaderProgram* shaderProgram;
	Audace::SimpleBillboardMaterial* material;
	Audace::Texture2d* cardTextures[52];
	std::vector<std::string> suitNames = { "Clubs", "Diamonds", "Hearts", "Spades" };
	std::vector<std::string> rankNames = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

	glm::vec2 mousePos;
	glm::vec2 grabOffset;
	bool dragging = false;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::ForwardCamera* camera;

	Audace::RandomUniformFloats rand;

public:
	DragDropScene(Audace::BaseAppController* controller, Audace::IFileAccess* fileLoader);
	Audace::Sprite* loadSprite(std::string filename);
	void loadAssets(Audace::IFileAccess* fileLoader) override;
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

	Audace::BaseCamera* getCamera() override;
	void setCamera(Audace::BaseCamera* camera) override;
};

#endif