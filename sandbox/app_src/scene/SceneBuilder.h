#ifndef AI_SCENEBUILDER_H
#define AI_SCENEBUILDER_H

#include "application/BaseAppController.h"
#include "FileLoader.h"
#include "scene/Scene.h"
#include "glm/glm.hpp"
#include "renderer/ShaderProgram.h"
#include "renderer/Sprite.h"
#include "scene/ForwardCamera.h"
#include "scene/BasicCameraController.h"

class SceneBuilder : public Audace::Scene
{
	Audace::FileLoader* fileLoader;
	glm::vec4 clearColor = glm::vec4(0, 0, 1, 0);
	Audace::ShaderProgram* shaderProgram;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::ForwardCamera camera = Audace::ForwardCamera::standard(glm::vec3(0, 0, 1), 1280.0f, 720.0f);
	Audace::BasicCameraController camController = Audace::BasicCameraController(&camera);
	float cameraYaw = 0;
	float cameraPitch = 0;

	glm::vec4 diffuseLight = glm::vec4(1, 1, 1, 1);
	glm::vec3 lightPos = glm::vec3(0, 10, 0);

	Audace::Sprite* currSprite;
	glm::vec3 spritePos = glm::vec3(0, 5, 1);
	glm::vec3 spriteAngles = glm::vec3(0, 0, 0);
	glm::vec3 spriteScale = glm::vec3(1, 1, 1);

public:
	SceneBuilder(Audace::BaseAppController *controller, Audace::FileLoader *fileLoader) : Scene(controller), fileLoader(fileLoader) {}

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

	void setVelZ(float v)
	{
		cameraVel.z = v;
	}

	void rotateCamera(float x, float y, float z)
	{
		cameraYaw += z;
		cameraPitch += x;
	}

};

#endif