//
// Created by Suspense on 3/5/2023.
//

#ifndef AUDACE_SCENE_H
#define AUDACE_SCENE_H

#include <GLES3/gl32.h>

#include "renderer/DataBuffer.h"
#include "renderer/ShaderProgram.h"
#include "openxr/OpenxrView.h"

class Scene {
	float verts[12] = {-1.0f, -1.0f, 0.0f,
				   -1.0f, 1.0f, 0.0f,
				   1.0f, -1.0f, 0.0f,
				   1.0f, 1.0f, 0.0f,
	};
	Audace::DataBuffer *vertexBuffer;
	GLuint vertexArray;
	Audace::ShaderProgram *shaderProgram;
	GLuint mvpMatLocation;
	GLuint colorLocation;

public:
	void init();
	void render(OpenxrView view);
};


#endif //AUDACE_SCENE_H
