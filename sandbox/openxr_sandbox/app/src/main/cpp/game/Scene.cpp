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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"

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

	quadMesh = Audace::Shapes::squarePositions();
	font = new Audace::BitmapFont(fileLoader, "arial.ttf");

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
	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Sprite *cliff_scene = loadSprite("cliff_scene.obj");

	{
		Audace::Sprite *sprite = cliff_scene->clone();
		glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
		sprite->setModelMatrix(modelMat);
		sprite->setPosition(glm::vec3(0, 0, -1));
		sprites.push_back(sprite);
	}
}

Audace::Sprite *Scene::loadSprite(std::string filename) {

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Model *model = fileLoader->readModelFile("models/", filename);
	for (Audace::ModelSection *section: model->sections) {
		section->material->setShader(shaderProgram);
	}
	Audace::Sprite *sprite = new Audace::Sprite(model);
	delete model;
	return sprite;
}

void Scene::render(OpenxrView view) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	AU_CHECK_GL_ERRORS();
	XrPosef pose = view.getViewData().pose;
	XrFovf fov = view.getViewData().fov;

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

	glm::mat4 camMat = glm::mat4_cast(glm::quat(pose.orientation.w, pose.orientation.x, pose.orientation.y, pose.orientation.z));
	camMat = glm::transpose(camMat);
	camMat = glm::translate(camMat, -glm::vec3(pose.position.x, pose.position.y, pose.position.z));
	glm::mat4 projMat = glm::frustum(tan(fov.angleLeft), tan(fov.angleRight), tan(fov.angleDown), tan(fov.angleUp), 1.0f, 100.0f);
	glm::mat4 vpMat = projMat * camMat;
	shaderProgram->setUniformMat4("vpMat", glm::value_ptr(vpMat));

//	{
//		// the aim pose
//		glm::mat4 worldMat = glm::translate(glm::mat4(1.0f), aimPose.position);
//		worldMat = worldMat * glm::mat4_cast(aimPose.orientation);
//		worldMat = glm::scale(worldMat, glm::vec3(0.02f, 0.02f, -2.0f));
//		worldMat = glm::translate(worldMat, glm::vec3(-0.5f, -0.5f, 0));
//		shaderProgram->setUniformMat4("worldMat", worldMat);
//		shaderProgram->setUniformVec2("textureScale", 1, 1);
//		AU_CHECK_GL_ERRORS();
//
//		shaderProgram->setUniformInt("tex1", 1);
//
//		boxSprite->render();
//		AU_CHECK_GL_ERRORS();
//	}
	for (Audace::Sprite *sprite: sprites) {
		sprite->render();
	}

}

void Scene::renderUi(OpenxrView view) {
	bool checked;
	float f;
	if (view.getViewData().pose.position.x < 0.0f) {
		ImGui::Begin("Left window");
		ImGui::SetWindowPos(ImVec2(500 + 55, 800));
	} else {
		ImGui::Begin("Left window ");
		ImGui::SetWindowPos(ImVec2(500 - 55, 800));
	}
	ImGui::Text("This is a text");
	ImGui::Checkbox("check me out", &checked);
	ImGui::SliderFloat("float", &f, 0, 1);
	ImGui::End();

	font->renderText("Sphynx of black quartz, judge my vow.");
}