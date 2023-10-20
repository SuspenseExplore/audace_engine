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
	shaderProgram->setUniformVec3("textureScale", 20, 20, 1);

	Audace::AssetStore::getTexture("images/white.png")->bind(1);
	Audace::AssetStore::getTexture("images/grass_004/Grass004_1K-JPG_Color.jpg")->bind(20);
	Audace::AssetStore::getTexture("images/grass_004/Grass004_1K-JPG_AmbientOcclusion.jpg")->bind(21);
	Audace::AssetStore::getTexture("images/grass_004/Grass004_1K-JPG_NormalDX.jpg")->bind(22);
	Audace::AssetStore::getTexture("images/ground_067/Ground067_1K-JPG_Color.jpg")->bind(30);
	Audace::AssetStore::getTexture("images/ground_067/Ground067_1K-JPG_AmbientOcclusion.jpg")->bind(31);
	Audace::AssetStore::getTexture("images/ground_067/Ground067_1K-JPG_NormalDX.jpg")->bind(32);
	Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_Color.jpg")->bind(40);
	Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_AmbientOcclusion.jpg")->bind(41);
	Audace::AssetStore::getTexture("images/rocks_011/Rocks011_1K-JPG_NormalDX.jpg")->bind(42);
	Audace::AssetStore::getTexture("images/ground_051/Ground051_1K-JPG_Color.jpg")->bind(5);

	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("grass");
		mat->setShader(shaderProgram);
		mat->setAmbientColor({1, 1, 1});
		mat->setAmbientOcclusionMap(21);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(20);
		mat->setNormalMap(22);
		grassMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("dirt");
		mat->setShader(shaderProgram);
		mat->setAmbientColor({1, 1, 1});
		mat->setAmbientOcclusionMap(31);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(30);
		mat->setNormalMap(32);
		dirtMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("stone");
		mat->setShader(shaderProgram);
		mat->setAmbientColor({1, 1, 1});
		mat->setAmbientOcclusionMap(41);
		mat->setDiffuseColor({1, 1, 1});
		mat->setDiffuseMap(40);
		mat->setNormalMap(42);
		rockMaterial = mat;
	}
	{
		Audace::Material *mat = new Audace::Material();
		mat->setName("water");
		mat->setShader(shaderProgram);
		mat->setAmbientColor({0.2f, 0.2f, 0.2f});
		mat->setAmbientOcclusionMap(1);
		mat->setDiffuseColor({0.690196f, 0.956863f, 1.000000f});
		mat->setDiffuseMap(1);
		waterMaterial = mat;
	}

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	Audace::Sprite *sprite = loadSprite(fileLoader, "bridge_stoneRoundNarrow.obj");
	glm::mat4 modelMat = glm::rotate(glm::scale(IDENTITY_MAT, {5, 5, 5}), glm::radians(90.0f), glm::vec3(1, 0, 0));
	// modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));
	sprite->setModelMatrix(modelMat);
	sprite->setPosition({0, 0, 0});
	sprites.push_back(sprite);

	pointLights = new Audace::PointLight[4];
	pointLights[0].setPosition({2, 0, 1.5f});
	pointLights[0].setColor({1, 1, 0.8f});
	pointLights[0].setIntensity(1);
	sprites.push_back(&pointLights[0]);

	pointLights[1].setPosition({-2, -2, 0.3f});
	pointLights[1].setColor({1, 0.2f, 0.1f});
	pointLights[1].setIntensity(0.6f);
//	sprites.push_back(&pointLights[1]);
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

void MainScene::enableAmbientOcclusion(bool enable)
{
	int c = enable ? 1 : 0;
	glm::vec3 color = {c, c, c};
	grassMaterial->setAmbientColor(color);
	dirtMaterial->setAmbientColor(color);
	rockMaterial->setAmbientColor(color);
}

void MainScene::render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", 0.01f, 0.01f, 0.02f, 1);
	shaderProgram->setUniformVec3("light[0].position", pointLights[0].getPosition());
	shaderProgram->setUniformFloat("light[0].intensity", pointLights[0].getIntensity());
	shaderProgram->setUniformVec3("light[0].color", pointLights[0].getColor());
//	shaderProgram->setUniformVec3("light[1].position", pointLights[1].getPosition());
//	shaderProgram->setUniformFloat("light[1].intensity", pointLights[1].getIntensity());
//	shaderProgram->setUniformVec3("light[1].color", pointLights[1].getColor());
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