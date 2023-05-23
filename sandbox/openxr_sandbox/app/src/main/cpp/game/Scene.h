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

class Scene {
	Audace::FileLoader *fileLoader;
	Audace::Sprite *circleSprite;
	Audace::ShaderProgram *shaderProgram;
	Audace::Texture2d *texture;

public:
	void init(AAssetManager *assetManager);
	void render(OpenxrView view);
};


#endif //AUDACE_SCENE_H
