#ifndef AI_SCENEBUILDER_H
#define AI_SCENEBUILDER_H

#include <vector>
#include <map>
#include "application/BaseAppController.h"
#include "FileLoader.h"
#include "scene/Scene.h"
#include "glm/glm.hpp"
#include "renderer/Sprite.h"
#include "renderer/material/Material.h"
#include "renderer/light/PointLight.h"
#include "scene/ForwardCamera.h"
#include "scene/BasicCameraController.h"
#include "content/Model.h"
#include "renderer/light/PointLight.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SceneBuilder : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 1);
	Audace::FileLoader *fileLoader;

	static const int scenePathLength = 64;
	char sceneWritePath[scenePathLength];
	json jsonContent = {};
	json modelIndex;
	std::vector<std::string> indexPath = {};

	Audace::BaseCamera *camera;

	Audace::ShaderProgram *shader;
	glm::vec4 ambientColor = {0.5, 0.5, 0.5, 1.0};
	Audace::PointLight *pointLight;
	glm::mat4 modelMat;

	Audace::Sprite *currSprite = nullptr;
	int nextSpriteId = 0;

public:
	SceneBuilder(Audace::BaseAppController *controller)
		: Scene(controller)
	{
		strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
	}

	void loadModel(std::string path, std::string filename);
	void traverseModelIndex(json index, int i);

	void loadAssets(Audace::FileLoader *fileLoader) override;
	void render() override;
	void renderUi() override;
	void disposeAssets() override;
	void setCamera(Audace::BaseCamera *camera) override { this->camera = camera; }
	Audace::BaseCamera *getCamera() override { return camera; }
	void teleport(glm::vec3 pos) override { camera->setOriginPos(pos); }
};

#endif