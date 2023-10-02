#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include <vector>
#include "glm/glm.hpp"
#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"
#include "scene/BaseCamera.h"
#include "application/BaseAppController.h"

class MainScene : public Audace::Scene
{
	std::vector<Audace::Sprite *> sprites;
	Audace::ShaderProgram *shaderProgram;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::BaseCamera *camera;

	glm::vec4 diffuseLight = glm::vec4(1, 1, 1, 1);
	glm::vec3 lightPos = glm::vec3(0, 0, 7);

public:
	MainScene(Audace::BaseAppController *controller) : Scene(controller) {}
	Audace::Sprite *loadSprite(Audace::FileLoader *fileLoader, std::string filename);
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

	// void setLightBright(bool bright)
	// {
	// 	diffuseLight.w = bright ? 2 : 1;
	// }

	// void randomLightColor()
	// {
	// 	glm::vec3 v = glm::normalize(glm::vec3(rand.get(), rand.get(), rand.get()));
	// 	diffuseLight = glm::vec4(v, diffuseLight.w);
	// }

	// void setLightPos(float x, float y)
	// {
	// 	lightPos = glm::vec3((x - 640.0f) / 50.0f, (y - 360.0f) / -50.0f, lightPos.z);
	// }

	// void rotateCamera(float x, float y, float z)
	// {
	// 	cameraYaw += z;
	// 	cameraPitch += x;
	// }

	Audace::BaseCamera* getCamera() override {return camera;}
	void setCamera(Audace::BaseCamera *camera) override {this->camera = camera;}
};

#endif