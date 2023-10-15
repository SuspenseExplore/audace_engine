#include "au_renderer.h"
#include "MainScene.h"
#include "ImageData.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "content/AssetStore.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Sprite.h"
#include "renderer/material/Material.h"
#include "util/StringUtil.h"
#include "imgui.h"
#include "SceneEnum.h"

void MainScene::loadAssets(Audace::FileLoader *fileLoader)
{
	glClearColor(0, 0, 1, 0);

	shaderProgram = Audace::AssetStore::getShader("standard");
	shaderProgram->bind();
	shaderProgram->setUniformVec3("textureScale", 10, 10, 1);

	Audace::AssetStore::getTexture("images/white.png")->bind(1);
	Audace::AssetStore::getTexture("images/grass_004/Grass004_1K-JPG_Color.jpg")->bind(2);
	Audace::AssetStore::getTexture("images/ground_067/Ground067_1K-JPG_Color.jpg")->bind(3);
	Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Color.jpg")->bind(4);
	Audace::AssetStore::getTexture("images/ground_051/Ground051_1K-JPG_Color.jpg")->bind(5);

	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("grass");
		mat->setShader(shaderProgram);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(2);
		grassMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("dirt");
		mat->setShader(shaderProgram);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(3);
		dirtMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("stone");
		mat->setShader(shaderProgram);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(4);
		rockMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("water");
		mat->setShader(shaderProgram);
		mat->setDiffuseColor({0.690196f, 0.956863f, 1.000000f});
		mat->setDiffuseMap(1);
		waterMaterial = mat;
	}

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Sprite *sprite = loadSprite(fileLoader, "cliff_scene.obj");
	glm::mat4 modelMat = glm::rotate(IDENTITY_MAT, glm::radians(90.0f), glm::vec3(1, 0, 0));
	// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
	sprite->setModelMatrix(modelMat);
	sprite->setPosition({0, 0, 0});
	sprites.push_back(sprite);

	pointLights = new Audace::PointLight[4];
	pointLights[0].setPosition({2, 0, 1});
	pointLights[0].setColor({1, 1, 0.8f});
	pointLights[0].setIntensity(1);
	sprites.push_back(&pointLights[0]);

	pointLights[1].setPosition({-2, -2, 0.3f});
	pointLights[1].setColor({1, 0.2f, 0.1f});
	pointLights[1].setIntensity(0.6f);
	sprites.push_back(&pointLights[1]);
}

Audace::Sprite *MainScene::loadSprite(Audace::FileLoader *fileLoader, std::string filename)
{
	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Model *model = fileLoader->readModelFile("models/", filename);
	for (Audace::ModelSection *section : model->sections)
	{
		Audace::Material *mat = reinterpret_cast<Audace::Material *>(section->material);
		mat->setShader(shaderProgram);
		if (Audace::StringUtil::startsWith(mat->getName(), "grass"))
		{
			section->material = grassMaterial;
		}
		else if (Audace::StringUtil::startsWith(mat->getName(), "dirt"))
		{
			section->material = dirtMaterial;
		}
		else if (Audace::StringUtil::startsWith(mat->getName(), "stone"))
		{
			section->material = rockMaterial;
		}
		else if (Audace::StringUtil::startsWith(mat->getName(), "water"))
		{
			section->material = waterMaterial;
		}
		else
		{
			mat->setDiffuseMap(1);
			AU_ENGINE_LOG_DEBUG(mat->getName());
		}
	}
	Audace::Sprite *sprite = new Audace::Sprite(model);
	delete model;
	return sprite;
}

void MainScene::render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shaderProgram->bind();
	shaderProgram->setUniformVec3("light[0].position", pointLights[0].getPosition());
	shaderProgram->setUniformFloat("light[0].intensity", pointLights[0].getIntensity());
	shaderProgram->setUniformVec3("light[0].color", pointLights[0].getColor());
	shaderProgram->setUniformVec3("light[1].position", pointLights[1].getPosition());
	shaderProgram->setUniformFloat("light[1].intensity", pointLights[1].getIntensity());
	shaderProgram->setUniformVec3("light[1].color", pointLights[1].getColor());
	shaderProgram->setUniformVec3("viewPos", camera->getPosition());

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