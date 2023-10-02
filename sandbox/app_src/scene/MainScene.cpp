#include "au_renderer.h"
#include "MainScene.h"
#include "ImageData.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Sprite.h"
#include "imgui.h"
#include "SceneEnum.h"

float verts[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f};

void MainScene::loadAssets(Audace::FileLoader *fileLoader)
{
	glClearColor(0, 0, 1, 0);

	std::string vs = fileLoader->textFileToString("shaders/standard/vs.glsl");
	std::string fs = fileLoader->textFileToString("shaders/standard/fs.glsl");
	shaderProgram = new Audace::ShaderProgram(vs, fs);
	shaderProgram->create();
	shaderProgram->bind();

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Sprite *sprite = loadSprite(fileLoader, "cliff_scene.obj");
	glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
	// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
	sprite->setModelMatrix(modelMat);
	sprite->setPosition({0, 0, 0});
	sprites.push_back(sprite);
}

Audace::Sprite *MainScene::loadSprite(Audace::FileLoader *fileLoader, std::string filename)
{
	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Model *model = fileLoader->readModelFile("models/", filename);
	for (Audace::ModelSection *section : model->sections)
	{
		section->material->setShader(shaderProgram);
	}
	Audace::Sprite *sprite = new Audace::Sprite(model);
	delete model;
	return sprite;
}

void MainScene::render()
{
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shaderProgram->bind();
	shaderProgram->setUniformVec3("light[0].position", lightPos.x, lightPos.y, lightPos.z);
	shaderProgram->setUniformFloat("light[0].intensity", 1.0f);
	shaderProgram->setUniformVec3("light[0].color", diffuseLight.x, diffuseLight.y, diffuseLight.z);
	shaderProgram->setUniformVec3("viewPos", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	shaderProgram->setUniformMat4("vpMat", camera->getViewProjMatrix());

	for (Audace::Sprite *sprite : sprites)
	{
		sprite->render(this);
	}

	ImGui::Begin("Scenes");
	if (ImGui::Button("Navigation"))
	{
		appController->setScene(SandboxScene::NAVIGATION);
	}
	ImGui::End();
}

void MainScene::disposeAssets()
{
	shaderProgram->destroy();
	delete shaderProgram;
}