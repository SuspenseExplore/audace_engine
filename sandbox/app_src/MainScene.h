#ifndef AU_MAINSCENE_H
#define AU_MAINSCENE_H

#include "glm/glm.hpp"
#include "scene/Scene.h"
#include "FileLoader.h"
#include "renderer/Sprite.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"
#include "scene/ForwardCamera.h"

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
	Audace::ForwardCamera camera = Audace::ForwardCamera::standard(glm::vec3(0, -10, 0), 1280.0f, 720.0f);
	float cameraYaw = 0;
	float cameraPitch = 0;

	glm::vec4 diffuseLight = glm::vec4(1, 0, 0, 1);
	glm::vec3 lightPos = glm::vec3(0);
	Audace::RandomUniformFloats rand = Audace::RandomUniformFloats::normalizedRange();

public:
	MainScene(Audace::FileLoader *fileLoader) : fileLoader(fileLoader) {}
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

	void setLightBright(bool bright)
	{
		diffuseLight.w = bright ? 2 : 1;
	}

	void randomLightColor()
	{
		glm::vec3 v = glm::normalize(glm::vec3(rand.get(), rand.get(), rand.get()));
		diffuseLight = glm::vec4(v, diffuseLight.w);
	}

	void setLightPos(float x, float y)
	{
		lightPos = glm::vec3((x - 640.0f) / 100.0f, (y - 360.0f) / -100.0f, 0);
	}

	void rotateCamera(float x, float y, float z) {
		cameraYaw += z;
		cameraPitch += x;
	}
};

#endif