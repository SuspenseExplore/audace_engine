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
#include "input/BooleanInputHandler.h"

class AppController;

class Scene {
	AppController *appController;
	Audace::FileLoader *fileLoader;
	Audace::Sprite *boxSprite;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *darkGridTex;
	Audace::Texture2d *greenChecksTex;
	Audace::Texture2d *orangeChecksTex;
	Audace::Texture2d *purpleChecksTex;

	Audace::BooleanInputHandler* lightOnAction;
	glm::vec3 lightPos;
	bool lightOn;

public:
	void init(AppController *controller, AAssetManager *assetManager);
	void setLightPos(glm::vec3 pos) {lightPos = pos;}
	void setLightOn(bool on) {lightOn = on;}
	void render(OpenxrView view);
};


#endif //AUDACE_SCENE_H
