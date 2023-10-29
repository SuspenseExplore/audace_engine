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

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SceneBuilder : public Audace::Scene
{
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 0);
	Audace::Mesh *quadMesh;
	Audace::FileLoader *fileLoader;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::BaseCamera *camera;
	float cameraYaw = 0;
	float cameraPitch = 0;

	Audace::Sprite *lightSprites[4];
	Audace::PointLight pointLights[4];

	int selectedModelIndex = 0;
	int modelCount;
	std::string modelBasePath;
	std::vector<std::string> modelFiles;

	Audace::Model *currModel;
	Audace::Sprite *currSprite;
	glm::vec3 spritePos = glm::vec3(0, 0, 0);
	glm::vec3 spriteAngles = glm::vec3(0, 0, 0);
	glm::vec3 spriteScale = glm::vec3(1, 1, 1);
	glm::mat4 modelMat = glm::mat4(1.0f);

	std::vector<Audace::Sprite *> sprites;

	glm::vec3 startPos = {0, 0, 0};
	Audace::Sprite *startPosMarker;

	static const int scenePathLength = 64;
	char sceneWritePath[scenePathLength];
	json jsonContent = {};

	Audace::Model *loadModel(std::string filename);
	void newScene();
	void saveScene(std::string filename);
	void loadScene(std::string filename);

public:
	SceneBuilder(Audace::BaseAppController *controller)
		: Scene(controller)
	{
		strcpy(sceneWritePath, "D:/audace_engine/sandbox/assets/scenes/MainScene.json");
	}

	void loadAssets(Audace::FileLoader *fileLoader) override;
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

	void setVelZ(float v)
	{
		cameraVel.z = v;
	}

	void rotateCamera(float x, float y, float z)
	{
		cameraYaw += z;
		cameraPitch += x;
	}

	void setCamera(Audace::BaseCamera *camera) override {this->camera = camera; }
	Audace::BaseCamera *getCamera() override { return camera; }
};

#endif