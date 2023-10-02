//
// Created by Suspense on 3/5/2023.
//

#ifndef AUDACE_SCENE_H
#define AUDACE_SCENE_H

#include <GLES3/gl32.h>
#include <jni.h>
#include <android_native_app_glue.h>
#include "scene/Scene.h"
#include "openxr/HmdCamera.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Texture2d.h"
#include "renderer/Sprite.h"
#include "renderer/text/BitmapFont.h"
#include "renderer/light/PointLight.h"
#include "openxr/OpenxrView.h"
#include "glm/glm.hpp"
#include "util/RandomUniformFloats.h"
#include "math/Pose.h"

class AppController;

class Scene : public Audace::Scene {
	AppController *appController;
	std::vector<Audace::Sprite*> sprites;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *darkGridTex;
	Audace::Texture2d *greenChecksTex;
	Audace::Texture2d *orangeChecksTex;
	Audace::Texture2d *purpleChecksTex;

	Audace::Mesh* quadMesh;
	Audace::BitmapFont* font;

	Audace::PointLight pointLights[4];
	bool lightOn;

	HmdCamera *camera;
	Audace::Pose aimPose;

	Audace::RandomUniformFloats rand = Audace::RandomUniformFloats::normalizedRange();

public:
	Scene(Audace::BaseAppController *controller) : Audace::Scene(controller) {} //TODO: nullptr is currently passed in from AppController.h
	void init(AppController *controller, AAssetManager *assetManager);
	Audace::Sprite* loadSprite(std::string filename);
	void setLightPos(glm::vec3 pos) {pointLights[0].position = pos;}
	void setLightOn(bool on) {lightOn = on;}
	void randomLightColor() {pointLights[0].color = glm::normalize(glm::vec3(rand.get(), rand.get(), rand.get()));}
	void setAimPose(Audace::Pose pose) {aimPose = pose;}
	void render(OpenxrView view);
	void renderUi(OpenxrView view);

	void loadAssets() override {}
	void render() override {}
	void disposeAssets() override {}

	void setCamera(HmdCamera *camera) {this->camera = camera;}
	Audace::BaseCamera* getCamera() override {return camera;}
};


#endif //AUDACE_SCENE_H
