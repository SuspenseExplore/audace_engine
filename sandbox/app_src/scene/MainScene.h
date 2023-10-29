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
#include "renderer/light/PointLight.h"
#include "renderer/material/Material.h"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"
#include "scene/BaseCamera.h"
#include "application/BaseAppController.h"

class MainScene : public Audace::Scene {
	std::vector<Audace::Sprite *> sprites;
	Audace::ShaderProgram *shaderProgram;

	Audace::Material *grassMaterial;
	Audace::Material *dirtMaterial;
	Audace::Material *rockMaterial;
	Audace::Material *waterMaterial;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::BaseCamera *camera;

	Audace::PointLight *pointLights;

public:
	MainScene(Audace::BaseAppController *controller) : Scene(controller) {}

	Audace::Sprite *loadSprite(Audace::FileLoader *fileLoader, std::string filename);

	void loadAssets(Audace::FileLoader *fileLoader) override;

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

	void enableAmbientOcclusion(bool enable);

	// void setLightBright(bool bright)
	// {
	// 	diffuseLight.w = bright ? 2 : 1;
	// }

	// void randomLightColor()
	// {
	// 	glm::vec3 v = glm::normalize(glm::vec3(rand.get(), rand.get(), rand.get()));
	// 	diffuseLight = glm::vec4(v, diffuseLight.w);
	// }

	void setLightPos(int index, glm::vec3 position) {
		pointLights[index].setPosition(camera->getOriginPos() + position);
	}

	void teleport() {
		// light pos includes origin already
		// offset = light - camera - origin
		// new origin = origin + offset
		// new origin = origin + (light - camera - origin)
]\
'['		// new origin = light - camera
		camera->setOriginPos(pointLights[0].getPosition() - camera->getPosition());
	}

	// void rotateCamera(float x, float y, float z)
	// {
	// 	cameraYaw += z;
	// 	cameraPitch += x;
	// }

	Audace::BaseCamera *getCamera() override { return camera; }

	void setCamera(Audace::BaseCamera *camera) override { this->camera = camera; }
};

#endif