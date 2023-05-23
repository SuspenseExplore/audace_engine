//
// Created by Suspense on 3/5/2023.
//

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "audace_common.h"
#include "openxr/xr_linear.h"
#include "ImageData.h"
#include "Scene.h"
#include "renderer/Shapes.h"

void Scene::init(AAssetManager *assetManager) {
	fileLoader = new Audace::FileLoader(assetManager);

	glClearColor(0, 0, 1, 1);
	glClearDepthf(1.0f);

	circleSprite = Audace::Shapes::cubePositions();

	std::string vs = fileLoader->textFileToString("shaders/color/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/color/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();

	Audace::ImageData img = fileLoader->readImageFile("images/backgroundColorGrass.png");
	texture = new Audace::Texture2d(img);
	texture->create();
}

void Scene::render(OpenxrView view) {
	shaderProgram->bind();
	XrPosef pose = view.getViewData().pose;

	XrMatrix4x4f projMat;
	XrMatrix4x4f_CreateProjectionFov(&projMat, GRAPHICS_OPENGL_ES, view.getViewData().fov, 0.1f, 100.0f);
	XrMatrix4x4f camMat;
	XrVector3f scale{1.f, 1.f, 1.f};
	XrMatrix4x4f_CreateTranslationRotationScale(&camMat, &pose.position, &pose.orientation, &scale);
	XrMatrix4x4f viewMat;
	XrMatrix4x4f_InvertRigidBody(&viewMat, &camMat);
	XrMatrix4x4f vpMat;
	XrMatrix4x4f_Multiply(&vpMat, &projMat, &viewMat);
	shaderProgram->setUniformMat4("vpMat", reinterpret_cast<float*>(&vpMat));
//	glUniformMatrix4fv(vpMatLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&vpMat));
	AU_CHECK_GL_ERRORS();

	glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0));
	worldMat = glm::rotate(worldMat, -glm::radians(90.0f), glm::vec3(1, 0, 0));
	worldMat = glm::scale(worldMat, glm::vec3(0.5f, 0.5f, 0.5f));
	shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
	AU_CHECK_GL_ERRORS();

	texture->bind(1);
	shaderProgram->setUniformInt("tex1", 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	AU_CHECK_GL_ERRORS();
	circleSprite->render();
	AU_CHECK_GL_ERRORS();
}