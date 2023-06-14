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
	{
		Audace::Model* model = fileLoader->readModelFile("models/", "cliff_blockSlopeWalls_rock.obj");
		for (Audace::ModelSection* section : model->sections) {
			section->material->setShader(shaderProgram);
		}
		modelSprite = new Audace::Sprite(model);
	}
	{
		Audace::Model* model = fileLoader->readModelFile("models/", "ground_pathStraight.obj");
		for (Audace::ModelSection* section : model->sections) {
			section->material->setShader(shaderProgram);
		}
		groundSprite = new Audace::Sprite(model);
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

	shaderProgram->bind();
	shaderProgram->setUniformVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
	shaderProgram->setUniformVec3("light.ambient", 0.2f, 0.2f, 0.4f);
	shaderProgram->setUniformVec3("light.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	shaderProgram->setUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);
	shaderProgram->setUniformVec3("viewPos", pose.position.x, pose.position.y, pose.position.z);

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
		// the aim pose
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), aimPose.position);
		worldMat = worldMat * glm::mat4_cast(aimPose.orientation);
		worldMat = glm::scale(worldMat, glm::vec3(0.02f, 0.02f, -2.0f));
		worldMat = glm::translate(worldMat, glm::vec3(-0.5f, -0.5f, 0));
		shaderProgram->setUniformMat4("worldMat", worldMat);
//		shaderProgram->setUniformVec2("textureScale", 1, 1);
		AU_CHECK_GL_ERRORS();

//		shaderProgram->setUniformInt("tex1", 1);

//		boxSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(10.0f, 10.0f, 10.0f));
		shaderProgram->setUniformMat4("worldMat", worldMat);
//		shaderProgram->setUniformVec2("textureScale", 10, 10);
		AU_CHECK_GL_ERRORS();

//		shaderProgram->setUniformInt("tex1", 1);

		groundSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0, 1));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
//		worldMat = glm::scale(worldMat, glm::vec3(10, 10, 10));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
//		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

//		shaderProgram->setUniformInt("tex1", 2);

		modelSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, -1));
		worldMat = glm::rotate(worldMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(2, 2, 2));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
//		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

//		shaderProgram->setUniformInt("tex1", 3);

		modelSprite->render();
		AU_CHECK_GL_ERRORS();
	}
	{
		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 5));
		worldMat = glm::rotate(worldMat, glm::radians(45.0f), glm::vec3(1, 0, 0));
		worldMat = glm::scale(worldMat, glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram->setUniformMat4("worldMat", glm::value_ptr(worldMat));
//		shaderProgram->setUniformVec2("textureScale", 2, 2);
		AU_CHECK_GL_ERRORS();

//		shaderProgram->setUniformInt("tex1", 4);

		modelSprite->render();
		AU_CHECK_GL_ERRORS();
	}
}