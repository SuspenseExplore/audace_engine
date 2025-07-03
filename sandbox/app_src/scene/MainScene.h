#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include <map>
#include <vector>
#include "glm/glm.hpp"
#include "scene/Scene.h"

namespace Audace
{
	class ShaderProgram;
	class Material;
	class PointLight;
}

class MainScene : public Audace::Scene {
	std::vector<Audace::Sprite*> sprites;
	Audace::ShaderProgram* shaderProgram;

	std::map<std::string, Audace::Material*> materials;

	glm::vec3 cameraVel = glm::vec3(0, 0, 0);
	Audace::BaseCamera* camera;

	glm::vec4 ambientLight = { 1, 1, 1, 0.2f };
	Audace::PointLight* pointLights;

public:
	MainScene(Audace::BaseAppController* controller);

	Audace::Sprite* loadSprite(Audace::IFileAccess* fileLoader, std::string filename);

	void loadAssets(Audace::IFileAccess* fileLoader) override;

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

	void setLightPos(int index, glm::vec3 position);
	void teleport();

	// void rotateCamera(float x, float y, float z)
	// {
	// 	cameraYaw += z;
	// 	cameraPitch += x;
	// }

	Audace::BaseCamera* getCamera() override;
	void setCamera(Audace::BaseCamera* camera) override;
};

#endif