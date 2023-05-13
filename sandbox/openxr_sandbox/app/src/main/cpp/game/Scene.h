//
// Created by Suspense on 3/5/2023.
//

#ifndef AUDACE_SCENE_H
#define AUDACE_SCENE_H

#include <GLES3/gl32.h>
#include <jni.h>
#include <android_native_app_glue.h>
#include "android_platform/FileLoader.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VertexArray.h"
#include "openxr/OpenxrView.h"

class Scene {
	float verts[12] = {-1.0f, -1.0f, -2.0f,
				   -1.0f, 1.0f, -2.0f,
				   1.0f, -1.0f, -2.0f,
				   1.0f, 1.0f, -2.0f,
	};
	Audace::FileLoader *fileLoader;
	Audace::DataBuffer *vertexBuffer;
	Audace::VertexAttribute *attr;
	Audace::VertexArray *vertexArray;
	Audace::ShaderProgram *shaderProgram;
	GLuint mvpMatLocation;
	GLuint colorLocation;

public:
	void init(AAssetManager *assetManager);
	void render(OpenxrView view);
};


#endif //AUDACE_SCENE_H
