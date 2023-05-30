//
// Created by Suspense on 3/5/2023.
//

#include <string>
#include <vector>
#include <chrono>
#include <functional>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "audace_common.h"
#include "openxr/xr_linear.h"
#include "AppController.h"
#include "ImageData.h"
#include "Scene.h"
#include "renderer/Shapes.h"

void Scene::init(AppController *controller, AAssetManager *assetManager) {
	appController = controller;
	fileLoader = new Audace::FileLoader(assetManager);

	glClearColor(0, 0, 1, 1);
	glClearDepthf(1.0f);

	boxSprite = Audace::Shapes::cubePosNorm();

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
		greenChecksTex = new Audace::Texture2d(img);
		greenChecksTex->create();
	}
	{
		Audace::ImageData img = fileLoader->readImageFile("images/orange_checkerboard.png");
		orangeChecksTex = new Audace::Texture2d(img);
		orangeChecksTex->create();
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
	XrPosef pose = view.getViewData().pose;

	darkGridTex->bind(1);
	greenChecksTex->bind(2);
	orangeChecksTex->bind(3);
	purpleChecksTex->bind(4);

	// make the light orbit a point over time
	float t = (float) std::chrono::high_resolution_clock::now().time_since_epoch().count();
	float x = cos(t / 5000000000.0f) * 7.0f;
	float y = sin(t / 5000000000.0f) * 7.0f;

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", 0.2, 0.2, 1, 0.2f);
	shaderProgram->setUniformVec4("diffusePos", lightPos.x, lightPos.y, lightPos.z, 0);
	shaderProgram->setUniformVec4("diffuseColor", lightColor.x, lightColor.y, lightColor.z, lightOn ? 2 : 1);
	shaderProgram->setUniformVec4("viewPos", pose.position.x, pose.position.y, pose.position.z, 0);

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
	AU_CHECK_GL_ERRORS();

	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(-10, -10, -2));
//		worldMat = glm::rotate(worldMat, glm::radians(0.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(20.0f, 20.0f, 1.0f));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 10, 10);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 1);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 2);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));
//		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 3);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 10));
//		worldMat = glm::rotate(worldMat, glm::radians(45.0f), glm::vec3(0, 0, 1));
		worldMat = glm::scale(worldMat, glm::vec3(5, 5, 5));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

		shaderProgram->setUniformInt("tex1", 4);

		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
}