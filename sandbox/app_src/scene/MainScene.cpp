#include "au_renderer.h"
#include "MainScene.h"
#include "ImageData.h"
#include "renderer/Shapes.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "content/AssetStore.h"
#include "content/JsonSerializer.h"
#include "renderer/DataBuffer.h"
#include "renderer/VertexAttribute.h"
#include "renderer/VertexArray.h"
#include "renderer/Sprite.h"
#include "renderer/material/Material.h"
#include "util/StringUtil.h"
#include "imgui.h"
#include "SceneEnum.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void MainScene::loadAssets(Audace::FileLoader *fileLoader)
{
	glClearColor(0, 0, 1, 0);

	shaderProgram = Audace::AssetStore::getShader("standard");
	shaderProgram->bind();
	shaderProgram->setUniformVec3("textureScale", 20, 20, 1);

	glm::mat4 IDENTITY_MAT = glm::mat4(1.0f);
	glm::mat4 modelMat = glm::rotate(glm::scale(IDENTITY_MAT, {1, 1, 1}), glm::radians(90.0f), glm::vec3(1, 0, 0));

	std::string s = fileLoader->textFileToString("scenes/MainScene.json");
	json jScene = json::parse(s);
	camera->setOriginPos({jScene["startPos"][0], jScene["startPos"][1], jScene["startPos"][2]});
	for (auto &item : jScene["materials"].items())
	{
		std::string name = item.key();
		std::string path = item.value();
		Audace::Material *mat = Audace::JsonSerializer::loadMaterial(fileLoader->textFileToJson(path));
		materials[name] = mat;
	}
	for (auto &item : jScene["sprites"].items())
	{
		std::string filename = item.key();
		json list = item.value();
		for (auto &it : list.items())
		{
			json obj = it.value();
			json pos = obj["position"];
			json angles = obj["orientation"];
			Audace::Sprite *sprite = loadSprite(fileLoader, filename);
			sprite->setModelMatrix(modelMat);
			sprite->setPosition({pos[0], pos[1], pos[2]});
			sprite->setOrientation(glm::quat(glm::radians(glm::vec3(angles[0], angles[1], angles[2]))));
			sprites.push_back(sprite);
		}
	}

	pointLights = new Audace::PointLight[4];
	pointLights[0].setPosition({2, 0, 5});
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
	Audace::Sprite *sprite = Audace::AssetStore::cloneSprite(filename);
	sprite->forEachMesh([this](Audace::Mesh *mesh)
						{
		Audace::Material *mat = reinterpret_cast<Audace::Material *>(mesh->getMaterial());
		mat->setShader(shaderProgram);
		for (auto &m : materials)
		{
			if (Audace::StringUtil::startsWith(mat->getName(), m.first))
			{
				mesh->setMaterial(m.second);
			}
		} });
	return sprite;
}

void MainScene::enableAmbientOcclusion(bool enable)
{
	ambientLight = enable ? glm::vec4(1.0f, 1.0f, 1.0f, 0.2f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void MainScene::render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->update();

	shaderProgram->bind();
	shaderProgram->setUniformVec4("ambientLight", ambientLight);
	shaderProgram->setUniformVec3("light[0].position", pointLights[0].getPosition());
	shaderProgram->setUniformFloat("light[0].intensity", pointLights[0].getIntensity());
	shaderProgram->setUniformVec3("light[0].color", pointLights[0].getColor());
	shaderProgram->setUniformVec3("light[1].position", pointLights[1].getPosition());
	shaderProgram->setUniformFloat("light[1].intensity", pointLights[1].getIntensity());
	shaderProgram->setUniformVec3("light[1].color", pointLights[1].getColor());
	shaderProgram->setUniformVec3("viewPos", camera->getPosition() + camera->getOriginPos());

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
}