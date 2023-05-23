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

	boxSprite = Audace::Shapes::cubePositions();

	std::string vs = fileLoader->textFileToString("shaders/standard/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/standard/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();

	{
		Audace::ImageData img = fileLoader->readImageFile("images/dark_grid.png");
		darkGridTex = new Audace::Texture2d(img);
		darkGridTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/green_checkerboard.png");
		orangeChecksTex = new Audace::Texture2d(img);
		orangeChecksTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/orange_checkerboard.png");
		greenChecksTex = new Audace::Texture2d(img);
		greenChecksTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/purple_checkerboard.png");
		purpleChecksTex = new Audace::Texture2d(img);
		purpleChecksTex->create();
	}
}

void Scene::render(OpenxrView view) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	AU_CHECK_GL_ERRORS();

	darkGridTex->bind(1);
	orangeChecksTex->bind(2);
	greenChecksTex->bind(3);
	purpleChecksTex->bind(4);

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", 1, 1, 0.6, 0.7f);

	XrPosef pose = view.getViewData().pose;
	XrMatrix4x4f projMat;
	XrMatrix4x4f_CreateProjectionFov(&projMat, GRAPHICS_OPENGL_ES, view.getViewData().fov, 0.1f,
									 100.0f);
	XrMatrix4x4f camMat;
	XrVector3f scale{1.f, 1.f, 1.f};
	XrMatrix4x4f_CreateTranslationRotationScale(&camMat, &pose.position, &pose.orientation, &scale);
	XrMatrix4x4f viewMat;
	XrMatrix4x4f_InvertRigidBody(&viewMat, &camMat);
	XrMatrix4x4f vpMat;
	XrMatrix4x4f_Multiply(&vpMat, &projMat, &viewMat);
	shaderProgram->setUniformMat4("vpMat", reinterpret_cast<float *>(&vpMat));
//	glUniformMatrix4fv(vpMatLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&vpMat));
	AU_CHECK_GL_ERRORS();

	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(-25, -25, -5));
		worldMat = glm::rotate(worldMat, glm::radians(0.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(100.0f, 100.0f, 0));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 10, 10);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 1);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 30, -5));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(10.0f, 10.0f, 20));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 2);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(20, 40, -5));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(0, 0, 1));
		worldMat = glm::scale(worldMat, glm::vec3(10.0f, 20.0f, 10));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 3);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(60, 0, -5));
		worldMat = glm::rotate(worldMat, glm::radians(45.0f), glm::vec3(0, 0, 1));
		worldMat = glm::scale(worldMat, glm::vec3(10.0f, 20.0f, 40));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 4);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
}