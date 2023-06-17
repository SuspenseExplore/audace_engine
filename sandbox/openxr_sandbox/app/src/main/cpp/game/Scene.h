//
// Created by Suspense on 3/5/2023.
//

#ifndef AUDACE_SCENE_H
#define AUDACE_SCENE_H

#include <GLES3/gl32.h>
#include <jni.h>
#include <android_native_app_glue.h>
#include "android_platform/FileLoader.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "renderer/Sprite.h"
#include "openxr/OpenxrView.h"
#include "glm/glm.hpp"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"

class AppController;

class Scene {
	AppController *appController;
	Audace::FileLoader *fileLoader;
	std::vector<Audace::Sprite*> sprites;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *darkGridTex;
	Audace::Texture2d *greenChecksTex;
	Audace::Texture2d *orangeChecksTex;
	Audace::Texture2d *purpleChecksTex;

	glm::vec3 lightPos;
	glm::vec3 diffuseLight = glm::vec3(1, 1, 1);
	bool lightOn;

	Audace::Pose aimPose;

	Audace::RandomUniformFloats rand = Audace::RandomUniformFloats::normalizedRange();

public:
	void init(AppController *controller, AAssetManager *assetManager);
	Audace::Sprite* loadSprite(std::string filename);
	void setLightPos(glm::vec3 pos) {lightPos = pos;}
	void setLightOn(bool on) {lightOn = on;}
	void randomLightColor() {diffuseLight = glm::normalize(glm::vec3(rand.get(), rand.get(), rand.get()));}
	void setAimPose(Audace::Pose pose) {aimPose = pose;}
	void render(OpenxrView view);
	void renderUi(OpenxrView view);
};


#endif //AUDACE_SCENE_H
